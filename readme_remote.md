# How to use iot-sdk
  
-------------------------------------
目录

1. [简介](#简介)
2. [下载 IDE](#下载ide)
3. [安装 IDE](#安装ide)
4. [获取 SDK](#获取sdk)
4. [导入工程](#导入工程)
5. [配置工程](#配置工程)
    1. [删除Include选项](#删除include选项) 
    2. [修改Language选项](#修改language选项)
    3. [更新到评估版本](#更新到评估版本)
5. [编译工程](#编译工程)
6. [下载与调试](#下载与调试)
7. [IoT示例使用流程](#iot示例使用流程)
    1. [准备工作](#准备工作)
    2. [启动配网](#启动配网)
    3. [控制设备](#控制设备)


## 简介
本 sdk 提供一个基于 MSP-EXP432P401R 开发板，通过与 Wi-Fi 模块进行 AT 指令串口通信，实现与阿里云 SDS 直连的 IoT 物联网典型开发应用示例源代码。

下面来讲解如何在您的 PC 上安装 CCS 开发环境，并在 CCS 中进行编译，下载本 sdk 工程。 



## 下载IDE
点击 [CCS 下载](http://processors.wiki.ti.com/index.php/XMS432_Support#Software_Compatibility)，进入开发工具下载列表页面。

在 “IDE Compatibility” 列表中，选择 “MSP432401R REVC” 选项中，适用您 PC 系统类型的 CCS 版本。如： Windows 系统 PC 开发环境，请下载： CCS Windows 对应的 CCS 6.1.1 .zip 文件。


![下载ccs](https://github.com/neooxu/IOT-MSP432/blob/master/image/download_ccs.png)


**注意：**下载需先登录。如无账号，请先注册，登录后，才能下载。



## 安装IDE
下载完成后，解压缩，双击 “ccs_setup_6.1.1.00022.exe” 启动安装。 其中：

1. 进入 “Processor Support” 页面, 请务必勾选 “MSP Ultra Low Power MCUS”，然后其它均默认，点击 “next” 即可。
2. 进入 “CCS Installation” 页面，启动安装，安装过程需耗费几分钟时间，请耐心等待。（请务必保证您的电脑保持联网状态。） 
3. 安装进程结束后，建议选择生成桌面快捷方式，以便开发时快速进入。


## 获取SDK
点击 [iot-sdk 下载](https://github.com/neooxu/IOT-MSP432/releases)，进入工程源代码的 github 仓库。 您需要：

下载 zip 或 tar.gz 压缩包文件到 PC 本地，“Download ZIP",下载后解压缩即可。




## 导入工程
双击桌面快捷方式 “Code Composer Studio 6.1.1”，进入 CCS 开发环境。

在 “Getting Started” 页面，依次点击 "Import Project"，再点击 “browse” 选择导入的工程文件夹：iot-sdk，路径如：F:\Git\IOT-MSP432\examples\MSP432P4xx\mxchip\iot_sdk，最后点击 “Finish” 即可。如下图所示。

![导入工程](https://github.com/neooxu/IOT-MSP432/blob/master/image/import_project.png) 

当出现下图提示时，请点击 "OK"，即可正常导入。

![继续导入](https://github.com/neooxu/IOT-MSP432/blob/master/image/import_OK.png) 


## 配置工程
为了在 CCS IDE 环境中正常编译 iot-sdk 文件，需对工程进行以下 3 步配置。

### 删除Include选项

* 左边栏 “Project Exploer” 选中工程 “iot_sdk”,右键选择 “Properties”,打开页面。  找到 “Build" —> "MSP432 Complier" —> "Include Options” ， 删除该页面的第一个path：“${CCS_BASE_ROOT}/arm/include”。

![删除一项](https://github.com/neooxu/IOT-MSP432/blob/master/image/ccsconfig_delete.png) 


### 修改Language选项

* 在工程 “Properties” 页面，找到  “Build” —> “MSP432 Complier” —> “Advanced Options” —>  “Language Options", 页面中 “
C Dialect"一栏， 请选择 “Complile program in C99 mode.(--c99)”,如下图所示。

![选择c99](https://github.com/neooxu/IOT-MSP432/blob/master/image/ccsconfig_choosec99.png) 

### 更新到评估版本

* 菜单栏中选择 “help" —> "Code Composer Studio Licensing Information", 进入  “License information view”页面，

![进入license修改页面](https://github.com/neooxu/IOT-MSP432/blob/master/image/ccsconfig_license_into.png) 

* 选择 “Upgrade" —> "Launch License Setup”，进入 “License Setup Wizard” 页面。

![license修改](https://github.com/neooxu/IOT-MSP432/blob/master/image/license_information.png) 

* 请选择 “EVALUATE” 选项，如下图。

![license修改](https://github.com/neooxu/IOT-MSP432/blob/master/image/ccsconfig_license_update.png) 

注意：每台 PC 仅限一次 License 更新机会，若二次安装 IDE，该 PC 便无法正常更新。

至此，工程已配置完成，可进行编译下载。


## 编译工程
点击工具栏中编译图标，开始工程编译过程，全程大约耗时几十秒，请耐心等待。

![开始编译](https://github.com/neooxu/IOT-MSP432/blob/master/image/compiler.png) 

在Console界面中查看编译结果，如图：表示编译成功，可以下载。

![编译完成](https://github.com/neooxu/IOT-MSP432/blob/master/image/compiler_ok.png) 



## 下载与调试

编译完成后，请使用 microUSB 连接线为开发板供电，并确认红色电源灯常亮，保证供电正常。 

此时可点击工具栏下载图标，开始下载。

![开始下载](https://github.com/neooxu/IOT-MSP432/blob/master/image/download.png) 

下载过程大约耗时 几秒钟，如下图：

![下载过程](https://github.com/neooxu/IOT-MSP432/blob/master/image/download_process.png) 

下载完成后， CCS 会自动跳转至 “在线调试” 状态，如下图所示：

![调试过程](https://github.com/neooxu/IOT-MSP432/blob/master/image/debug_status.png) 

此时，开发者可以进行 单步调试 等操作。

若要退出调试状态，请点击工具栏 红色正方形 图标，如下图。

![退出调试](https://github.com/neooxu/IOT-MSP432/blob/master/image/quit_debug.png) 



## IoT示例使用流程

本示例工程提供一个典型的 IoT 应用示例。 系统结构组成如下图：

![IOT典型应用系统组成](https://github.com/neooxu/IOT-MSP432/blob/master/image/demo_frame.png)

TI 的 MSP432P401R 单片机， 通过串口连接 EMW3080 Wi-Fi 模块，并与之进行 AT 指令通信，实现与阿里云 SDS 的通信，同时手机 APP 端可监控开发板的外部设备。

使用具体步骤如下：


### 准备工作

**—— 手机 APP 端**

1.下载安装APP: 点击 [示例 APP 下载页面](https://www.pgyer.com/TIAnd)，或扫描下方二维码，请根据手机系统类型选择下载。

![app下载](https://github.com/neooxu/IOT-MSP432/blob/master/image/app_address.png)  

2.使用手机号码，进行 APP 用户账号注册，验证，登录。

![app注册登录](https://github.com/neooxu/IOT-MSP432/blob/master/image/app_userid.png)

3.请确保手机已经成功连接至现场路由器。




**—— 开发板设备端**

1. 请使用 microUSB 连接线为开发板供电，并确认红色电源灯常亮，保证供电正常。

![设备供电](https://github.com/neooxu/IOT-MSP432/blob/master/image/device_power_on.png) 


### 启动配网
打开手机 APP, 点击右上角 “+” 号，选择设备 “TI开发板”，根据提示进行操作。

1. 长按开发板上盖板的 USER 按键，恢复设备出厂设置，此时开发板 OLED 屏最后一行将打印显示："Restore default",继而跳变至 “Wi-Fi config....” 等待手机配网中；
2. APP 配网输入界面中，设置正确的路由器用户名和密码，启动配网，此后开发板 OLED 屏最后一行将显示 设备的联网状态，
    1.  “Wi-Fi connected” 代表成功获取路由器的 SSID 和 密码
    2.  “Cloud connected” 表示与云服务器连接成功，此时需进行身份认证；
3. 当 APP 提示身份验证时，请短按上盖板的 USER 按键，完成身份认证；
4. 以上步骤完成后，APP 会自动跳转设备列表页面，显示在线设备；
5. 点击设备图标，进入设备控制页面。

![恢复出厂](https://github.com/neooxu/IOT-MSP432/blob/master/image/reset_device.png)——> ![启动配网](https://github.com/neooxu/IOT-MSP432/blob/master/image/net_config.png)

![身份验证](https://github.com/neooxu/IOT-MSP432/blob/master/image/id_ensure.png)  ——> ![设备列表](https://github.com/neooxu/IOT-MSP432/blob/master/image/device_list.png)



### 控制设备
进入设备控制页面，可对各外部设备参数进行监测或控制，或在线调试。

1. 监测：
   1. 温湿度值：设备向云端上报数据
   2. 开关状态：可通过拨动开发板上的拨码开关体验。 开关1：S1， 开关2： S2。 高电平为关，低电平为开。
2. 控制：
   1. RGB灯颜色，饱和度，亮度（通过滑动色彩环 或 滑动条来调整）。
3. 在线调试：
   1. APP 调试界面向设备发送字符串，会在 OLED 屏最后一行打印出来（注意屏幕打印不支持中文）。
   2. APP 发送数据会同步显示在接收区，带来 echo 回声体验效果。


![设备监控](https://github.com/neooxu/IOT-MSP432/blob/master/image/device_control.png) ——>  ![在线调试](https://github.com/neooxu/IOT-MSP432/blob/master/image/app_debug.png)


## 结束语

以上为本 SDK 在 CCS 开发环境中的具体使用方法，请仔细参考，祝学习愉快，谢谢。
  





