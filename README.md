# Sigheath
making a project to control servers.

##重新开始一个工程（19.10.25）
###前言
1. 由于一些原因，主要是因为我比较懒，没有进行整理。原来的项目目标没有实现，只留下好多散乱的代码，这段时间想整理一下这些代码，并使用C++来进行代码重写
2. 实现的目标是能够实现对服务器状态的实时监控，以保证服务器的平稳运行
3. 找了一个小伙伴，希望能一起实现


###大体结构描述
1. baseTools 基础的函数工具包，baseTools来实现配置文件的读取
2. sigNet 基类，实现了Socket网络函数的相关封装，并做为两端基类
3. manager manager端实现链接servers，并接收server相关系统信息
4. server server端检查本机情况，链接并发送相关系统信息

###文件结构
<table>
	<tr>
		<th rowspan="16">Sighealth</th>
		<td rowspan="11">src</td>
		<td>script</td>
	</tr>
	<tr><td>include:基础文件头</td><td>[*.h]</td></tr>
	<tr><td rowspan="3">sigToolkit:sig工具包</td><td bgcolor="yellow">baseTools.cpp</td></tr>
	<tr><td>confException.cpp</td></tr>
	<tr><td bgcolor="lightblue">sigNet.cpp[基类]</td></tr>
	<tr><td rowspan="1">server</td><td>conf</td><td>server.conf</td></tr>
	<tr><td rowspan="3">manager</td><td>conf</td><td>manager.conf</td></tr>
	<tr><td bgcolor="orange">manager.cpp <b>[main]</b></td></tr>
	<tr><td bgcolor="pink">Makefile => manager</td></tr>
	<tr><td rowspan="2">old:旧版文件</td><td>client.c</td></tr>
	<tr><td>mynet.h</td></tr>
	<tr><td rowspan="5">log</td><td>manager:</td><td>[servername]</td></tr>
	<tr><td rowspan="4">server:</td><td>cpu.log</td></tr>
	<tr><td>mem.log</td></tr>
	<tr><td>disk.log</td></tr>
	<tr><td>net.log</td></tr>
	<tr><td>Makefile</td></tr>
	<tr><td>README.md</td></tr>
</table>

###sigToolkit
1. 提供配置文件读写函数
2. 提供网络链接基类

####配置优化点
1. 将配置文件加载到内存替代每次都读内存【STL boost】

###manager端
1. 读取配置文件[基本实现]
2. 进行网络链接[未完成]

####配置与优化点
1. 使用lambda表达式与该智能指针将要析构时可以赋值操作的性质，替换使用move()【智能指针（unique_ptr）】
2. 使用try-catch机制简化代码异常判断流程。【异常处理】

###server端