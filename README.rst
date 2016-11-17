
PyVIX
=====

Python bindings for VMware products.

OS X
    $ install_name_tool -change libvixAllProducts.dylib \
        /Applications/VMware\ Fusion.app/Contents/Public/libvixAllProducts.dylib \
        <path/to/python3.5/site-packages/pyvix.cpython-35m-darwin.so>
