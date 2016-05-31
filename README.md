### 准备材料
* LinkNode D1 x 1   
* Infrared Shield x 1  
* 带遥控器的设备 x 1  

### 注册登录[LinkSprite.io](www.linksprite.com)
网页进入www.linksprite.io注册一个账号，并登录，在自己的账号下面建一个DIY设备，设备类型为00(Custom device type),设备分组为Remote，设备名可以自己DIY。   
* 注册[www.linksprite.io](www.linksprite.com)
* 登录此账号  
* 创建一个设备
* 获取apikey
* 获取deviceID   
![注册LinkSprite设备](picture/1.png)  
![获取deviceID](picture/2.png)  
![获取apikey](picture/3.png)  
 
### github获取代码
将github上获取的到文件夹放到Arduino IDE编译工具文件夹目录下的libraries目录下。打开Aeduin IDE工具选择我们刚刚添加的库目录下的D1_remote打开，修改相应的deviceID和apikey.
* 获取代码
* 放到Arduino ID的librarys目录
* 修改deviceID和apikey
* 连接LinkNode D1与PC
* 编译上传程序至LinkNode D1

### github获取app并制作顶一个遥控器
<p>首先确认手机有红外发射功能方可正常使用软件</p>
* 获取APP
* 账户注册、登陆
* Linksprite设备管理
* Linksprite设备添加
* 遥控器管理
* 遥控器录入、使用

</br>1. 下载地址</br>
安卓端下载，在github上获取[remotecontrol.apk](www.linksprite.com)安装包

</br>2. 下载安卓后打开软件登陆账户，未注册用户，需先注册方能登陆使用，如图所示：</br>
![用户登陆](picture/5.png)

</br>3. 登陆成功后,进入Linksprite设备列表，可进行Linksprite设备的注册</br>
![Linksprite设备列表](picture/6.png)

</br>4. 点击右上角添加按钮，可对Linksprite设备二维码进行扫描，扫描成功后，添加备注名称，进行注册</br>
 ![Linksprite设备二维码扫描注册](picture/7.png)

</br>5. 点击已经注册的Linksprite设备，显示遥控器列表，可添加多个遥控器，长按遥控器列表，可对遥控器的备注进行修改，保存在本地</br>
![Linksprite设备遥控器管理](picture/8.png)

</br>6. 选择遥控器，长按按钮进入遥控器键值录入，录入成功的按钮将被激活（如没有录入的按钮置灰），可实现遥控器的功能</br>
![遥控器录入、使用](picture/9.png)
