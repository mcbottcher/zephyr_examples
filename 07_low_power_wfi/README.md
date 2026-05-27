# 07 Low Power WFI

Button-driven LED toggle using a GPIO interrupt and the system workqueue. The main thread exits after setup; the idle thread runs WFI whenever nothing is scheduled.

## How it works

1. Button press fires a GPIO interrupt (ISR)
2. ISR submits a work item to the system workqueue (this is done in the gpio-keys driver)
3. Workqueue thread toggles the LED
4. With no threads runnable, the idle thread executes `__WFI`

## Where WFI is called

The idle thread in `kernel/idle.c:75` calls `k_cpu_idle()`, which resolves to
`arch_cpu_idle()` in `arch/arm/core/cortex_m/cpu_idle.c:99`, where the actual
`__WFI` instruction is issued via `SLEEP_IF_ALLOWED(__WFI)`.

## Kconfig options that affect idle behaviour

| Option | Effect |
|--------|--------|
| `CONFIG_PM` | Replaces the direct `k_cpu_idle()` call with `pm_system_suspend()`, enabling deeper SoC sleep states (Stop, Standby). The idle thread passes it the time until the next kernel timeout. |
| `CONFIG_ARM_ON_ENTER_CPU_IDLE_HOOK` | Enables a hook `z_arm_on_enter_cpu_idle()` called just before WFI. If it returns false, WFI is skipped entirely. |
| `CONFIG_ARM_ON_ENTER_CPU_IDLE_PREPARE_HOOK` | Enables a preparatory hook `z_arm_on_enter_cpu_idle_prepare()` called before the above, used to set up data for it. |
| `CONFIG_ARM_ON_EXIT_CPU_IDLE` | Enables a SoC-provided hook injected immediately after the WFI instruction returns. |
| `CONFIG_ARCH_HAS_CUSTOM_CPU_IDLE` | Removes the default `arch_cpu_idle()` entirely, letting the application supply its own implementation. |

## Build, Flash, and Run

```bash
export ZEPHYR_BASE=~/zephyrproject/zephyr
west build --pristine --board stm32f3_disco .
west flash
```
