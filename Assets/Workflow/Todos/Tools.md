# Vantor Tools Todo Stack

## VTRG

### New Commands:

| Main Command | Meaning                     | Example                            |
| ------ | --------------------------- | ---------------------------------- |
| `config`    | Change Engine Configs, version settings, ... | `vtrg config --version 0.16.4`, `vtrg config apply`|
| `build`   | Build Library, Samples, Sanbox       | `vtrg build --sample TestFramework --platform Windows`|
| `connect`   | Connect Interface to Application or Device   | `vtrg connect --platform Switch` |
| `deploy`   | Push changes automatically to github and save in logs | `vtrg deploy *`, `vtrg deploy <ModuleName>` |
| `format`   | Format CodeBase with given Configs  | `vtrg format *`, `vtrg format <ModuleName>`                 |
| `apply`   | Apply Config changes | `vtrg config apply`|
| `test`   | Run a Test for a Module                       | `vtrg test <ModuleName>` |
| `studio`   | Studio Application (Reserved but not in use) | `vtrg studio`|
| `whomai`   | See your DevEnv role (Reserved but not in use) | `vtrg whoami`|

### New Flags:

 | Flag | Meaning                     |
| ------ | --------------------------- |
| `--platform`    | Set build platform target |
| `--debug`   | Set Debug Build      |
| `--version`   | See version of vtgr and vantor   |
| `--help`   | See all vtrg commands and flags |
| `--release`   | Set Release Build  |
| `--clean`   | Clean old builds |
| `--list`   | list all build modules, platforms, etc.|
| `--profile`   | Profile Build  (Reserved but not in use)|
| `--skip-assets`   | Skip Asset Compilation during build (Reserved but not in use)|