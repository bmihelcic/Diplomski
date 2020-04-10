# Diplomski

1. Install wxWidgets
2. add wxWidgets installation directory as "WXWIN" environment variable 

Project settings (Visual Studio):
 - Configuration: All Configuration
 - C/C++ -> General -> Additional Include Directories: $(WXWIN)\include;$(WXWIN)\include\msvc
 - C/C++ -> Precompiled Headers -> Precompiled Header: Not Using Precompiled Headers
 - Linker -> General -> Additional Library Directories: $(WXWIN)\lib\vc_lib  