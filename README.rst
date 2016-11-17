
PyVIX
=====

Python bindings for VMware products.

On macOS you may receive the following error:
::
    import pyvix
    Traceback (most recent call last):
      File "<stdin>", line 1, in <module>
    ImportError: dlopen(/Library/Frameworks/Python.framework/Versions/3.5/lib/python3.5/site-packages/pyvix.cpython-35m-darwin.so, 2): Library not loaded: libvixAllProducts.dylib
      Referenced from: /Library/Frameworks/Python.framework/Versions/3.5/lib/python3.5/site-packages/pyvix.cpython-35m-darwin.so
      Reason: image not found

It can be fixed by running the following command:
::
    $ install_name_tool -change libvixAllProducts.dylib /Applications/VMware\ Fusion.app/Contents/Public/libvixAllProducts.dylib /path/to/python3.5/site-packages/pyvix.cpython-35m-darwin.so
