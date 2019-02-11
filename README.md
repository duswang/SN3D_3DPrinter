
# **SN3D Project**

[![build](https://img.shields.io/badge/build-passing-brightgreen.svg)](https://bitbucket.org/xengiennering/sn3d-project)  [![platform](https://img.shields.io/badge/platform-linux-lightgrey.svg)]()
### **Package**
[![cmake](https://img.shields.io/badge/cmake-2.6-green.svg)](https://cmake.org/) [![gcc](https://img.shields.io/badge/gcc-6.3.0-green.svg)](https://gcc.gnu.org/)

### **Library**
 [![libconv](https://img.shields.io/badge/libiconv-1.15-green.svg)](https://github.com/roboticslibrary/libiconv) [![libusb](https://img.shields.io/badge/libusb-1.0.22-green.svg)](https://libusb.info/) [![libzip](https://img.shields.io/badge/libzip-1.5.1-green.svg)](https://libzip.org/) [![libpng](https://img.shields.io/badge/libpng-1.6.28-green.svg)](http://www.libpng.org/pub/png/libpng.html) [![libxml2](https://img.shields.io/badge/libxml2-2.9.7-green.svg)](http://www.xmlsoft.org/index.html) [![SDL2](https://img.shields.io/badge/SDL2-2.0.8-red.svg)](https://www.libsdl.org/download-2.0.php)

### **Used Editor**

[Mars Eclipse for mac](https://www.eclipse.org/mars/)

# **Features**
---

> This README.md uses the [Markdown](http://daringfireball.net/projects/markdown/) syntax. The [MarkDownDemo tutorial](https://bitbucket.org/tutorials/markdowndemo) shows how various elements are rendered. The [Bitbucketdocumentation](https://confluence.atlassian.com/bitbucket/readme-content-221449772.html) has more information about using a README.md.


# **Install & Compile**
---
#### **_Step 1._** Clone git repository.
```
$ git clone https://bitbucket.org/xengiennering/sn3d-project.git
```

#### **_Step 2._** Make makeFilk by cmake and check file list.
```
$ cmake CmakeList.txt
$ ls -al
```

#### **_Step 3._** SN3D Project Install to root directory.
```
$ make sn3d_install
$ cd /SN3D/sn3d-project
```

#### **_Step 4._** Install Systemd Service & other setting.
```
$ make sn3d_service_install
$ sudo reboot
```

#### **_Step 5._** Check is service working.
```
$ journalctl -u SN3D_Bootloader.service
$ journalctl -u SN3D.service
```

# **Make Commands**
---
#### Install 
`$ make sn3d_install` - SN3D Repository install to root directory => `/SN3D/sn3d-project`.

`$ make sn3d_service_install` - SN3D.service & SN3D_Bootloader.service install to systemd.

#### Systemd Service
`$ make start ` - SN3D.service restart.

`$ make stop ` - SN3D.service stop 

#### Build
`$ make all` - Build project.

`$ make clean` - Clean project. ( not remove cmakeFiles just build file. )

`$ make all_clean` - build file and all cmake file is remove. 

#### Run & Debug
`$ make gdb` - SN3D start with GDB. First do `$ make stop`.

`$ make run` - SN3D start without GDB. First do `$ make stop`.

#### API Document
`$ make doxygen` - make SN3D API Html file path(`/SN3D/sn3d-project/doc/html`)

# **CMake -** [![cmake](https://img.shields.io/badge/cmake-2.6-green.svg)](https://cmake.org/)
---
#### CMake build.
```
$ cmake CmakeList.txt
```
# **Sprinter**
---
> The leading developers of Sprinter are currently Kliment and caru, though many others contribute with their patches. This is a firmware for RAMPS and other reprap single-processor electronics setups. It supports printing from SD card, active heatbed control, and ATmega internal pullups. This work is licensed under the GNU GPL v3 or (at the user's discretion) any later version. It is based on Tonokips's firmware, which was licensed under GPL v2 or later.

See [Sprinter](https://github.com/kliment/Sprinter) on Github.

### G Code
`M106 S255` - Turn on UV lamp.

`M107`      - Turn off Uv lamp.

`M92`       - Set current position.

`G1`        - Move to by prameter.

`G28 Z0`    - Homing.

> or More detail & other G Code click  =>  [Reprap G Code Sheet](https://reprap.org/wiki/G-code) 


# **Nextion Display**
---

* #### Used Models
    - NX4024T032_011R
    - NX4827T043_011R
    - NX8048T050_011R
    - NX8048T070_011R

> or More detail click => [Nextion HMI Solution](https://www.itead.cc/wiki/Nextion_HMI_Solution) 

* ### [Nextion Editor](https://nextion.itead.cc/resources/download/nextion-editor/)
    * #### [Nextion Serial Commands](https://nextion.itead.cc/resources/documents/instruction-set/)
    * #### [Tools](https://nextion.itead.cc/resources/download/tooles-pages/)

* ### HMI Option 
    - #### [EUC-KR](https://en.wikipedia.org/wiki/Extended_Unix_Code#EUC-KR) - **Need covert to EUC-KR from UTF-8 format.**

# **Reference**
---
* ### Software

* ### Hardware
    - [Sprinter](https://github.com/kliment/Sprinter)
    - Raspberry Pi 3 B+
    - [Ramps Board](https://reprap.org/wiki/RAMPS_1.4) 

* ###  Linux OS
    - [Raspbian Stretch Lite](https://www.raspberrypi.org/downloads/raspbian/)

#### TODO List
- [X] Memory Test.
- [ ] PNG image checker & notify to user.
- [ ] Info screen design.
- [X] 8.9 Inch Display Test

#### BUG List
- [ ] USB mount event.
- [ ] Time indicate timer stoped when printing ( but it restart when resume ).


