# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

`pyvix` is a single CPython C extension module (no Python sources) that wraps VMware's VIX C API. Project metadata, including the version, lives in `pyproject.toml` (setuptools backend). `setup.py` only defines the extension and links it against the VIX library that ships with VMware Workstation / Fusion / Player.

## Build and run

```sh
pip install -e .                       # editable install into the current env
pip install .                          # regular install
python3 setup.py build_ext --inplace   # quick rebuild of pyvix.*.so in the repo root
python -m build                        # sdist + wheel into dist/ (needs the `build` package)
python3 examples/list.py               # smoke test: connect and list running VMs
```

- There is no test suite, linter, or CI. `examples/list.py` is the only way to exercise the module, and it needs a running VMware product with VMs.
- `pyvix.h` is listed in the extension's `depends`, so editing it triggers a rebuild, and that's also how it gets into the sdist. Changes to `setup.py` don't trigger one: run `python3 setup.py build_ext --force`, or `pip install .` reuses the stale objects in `build/`.
- If the `build` package isn't installed, running `python -m build` from the repo root imports the `build/` directory instead and fails with `No module named build.__main__`.
- `examples/list.py` passes against Fusion 26.0.1 on macOS arm64.
- The VIX headers and library must be installed. `setup.py` hard-codes their locations per OS:
  - **macOS**: `/Applications/VMware Fusion.app/Contents/Public` (`include/` and `libvixAllProducts.dylib`). The dylib's install name is a bare `libvixAllProducts.dylib`, so a custom `build_ext` runs `install_name_tool` after linking to point the `.so` at the absolute path inside Fusion.
  - **Linux**: `/usr/include/vmware-vix` and `/usr/lib/vmware-vix/lib` (links `vixAllProducts` and `dl`).
  - **Windows**: `%PROGRAMFILES(x86)%\VMware\VMware VIX` (64-bit uses `Vix64AllProductsDyn`).
- Recent builds target arm64 macOS with CPython 3.13 and 3.14 (see `build/`). `deb_dist/` in `.gitignore` is the output of Debian packaging (stdeb).

## Architecture

- `src/pyvix.h`: declares the two object structs (`PyVixHost` holds `VixHandle host`, `PyVixVM` holds `VixHandle vm`), `PyVix_Error`, and the shared helpers. Its `#ifndef VIX_SERVICEPROVIDER_DEFAULT` override always fires, because `vix.h` defines that name as an enum value, not a macro. So `pyvix.SERVICEPROVIDER_DEFAULT`, and the provider `connect()` uses by default, is 3 (Workstation), not VIX's own default of 1.
- `src/pyvix.c`: module init. It exports VIX constants as module ints and defines:
  - `pyvix.connect()`, which returns a `pyvix.host`. Its `options` argument defaults to `HOSTOPTION_DEFAULT` (`PYVIX_HOSTOPTION_DEFAULT` in `pyvix.h`), which is `0x200` on macOS (`__APPLE__`) and `0` elsewhere. `vix.h` doesn't document `0x200`, but it's what `vmrun` passes (seen in lldb). Without it, Fusion 26 fails every connect with `This operation is not supported with the current license`, whichever library or service provider is used. When adding flags, combine them with it (`HOSTOPTION_DEFAULT | HOSTOPTION_VERIFY_SSL_CERT`).
  - `VixDiscoveryProc`, the `FindItems` callback that appends VM paths to a Python list
  - `_PyVix_GetProperty`, the `property(id)` implementation shared by host and VM, which dispatches on `Vix_GetPropertyType`
- `src/pyvix_host.c`: the `pyvix.host` type. `open(vmx)` creates a `pyvix.vm`.
- `src/pyvix_vm.c`: the `pyvix.vm` type.

## Conventions in the C code

- **Object creation.** Types use `PyType_GenericNew`. Instances are made by calling the type object (`PyObject_CallObject(&PyVixHost_Type, NULL)`) and then assigning the VIX handle. `tp_init` resets the handle to `VIX_INVALID_HANDLE`, and `tp_dealloc` disconnects or releases it if it's still valid.
- **VIX call pattern.** Start a job, `VixJob_Wait`, then `Vix_ReleaseHandle(job)`, all inside `Py_BEGIN_ALLOW_THREADS` / `Py_END_ALLOW_THREADS`. On failure, raise `PyVix_Error` with `Vix_GetErrorText(error, NULL)`.
- **Invalid handles don't raise.** After `disconnect()`, the host's `running()` and `registered()` return `[]` and `open()` returns `None`, instead of raising. Only `property()` raises (`"Invalid handle"`). VM methods don't check the handle at all.
- **GIL in callbacks.** VIX callbacks such as `VixDiscoveryProc` run on a VIX thread while the GIL is released, so any Python C-API use inside them must be wrapped in `PyGILState_Ensure` / `PyGILState_Release`.
- **Constant names.** Constants are added to the `PyModule_AddIntConstant` block in `PyInit_pyvix`. Most just drop the `VIX_` prefix, but some don't: `SERVICEPROVIDER_SERVER`, `SERVICEPROVIDER_WORKSTATION` and `SERVICEPROVIDER_VI_SERVER` also drop `VMWARE_`, and `VMPOWEROP_GUI` comes from `VIX_VMPOWEROP_LAUNCH_GUI`. Match the existing Python names rather than renaming them.
- **Header compatibility.** Code has to compile against old and new VIX headers. For example, `IS_BOOL` vs `IsBool` is handled with `#ifdef`.

## Known quirks

- `host.registered()` raises `The operation is not supported` on Fusion. VIX only supports listing registered VMs on VMware Server.

- `PROPERTY_VM_POWER_STATE` can have extra, undocumented bits set, so test it with `&`, not `==`.
- On Ubuntu 18.04 with Python 3.6.7 and Workstation 15.0.2, `VixHost_Connect` corrupts internal state, and any unhandled Python exception then segfaults (from the README).
