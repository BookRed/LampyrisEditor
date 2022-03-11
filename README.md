# LampyrisEditor

欢迎来到BookRed的Github，您现在看到的是我个人独立自主开发的“游戏引擎” -- LampyrisEngine编辑器部分的代码。

代码主要分为两个部分，Editor部分与Runtime部分。

一，Editor部分主要实现了以下功能：
1）Hierarchy 管理窗口，负责管理场景中的所有实体。  
2）场景沙盒，可以在场景中进行漫游。  
3）属性编辑器，点击场景中的实体可以显示实体中的所有组件，实现原理是将QT的大部分控件进行immediate化，具体实现见le_editor_property_editor_widget.h  
4) Console窗口，可以打log。  

二，Runtime部分主要实现了以下内容：  
1）游戏底层类的定义，包括Entity,Component等。  
2）基于Vulkan的简单渲染器。  
3）对LEObject序列化与反序列化支持。  
4）简单的资源管理系统。  
