编译时候需要加上选项,配置如下
项目 -- 属性 -- 链接器 -- 命令行 -- 其它选项
	/INTEGRITYCHECK 

测试环境:
	win10  1903 (18362.30)

测试结果:
	win10中拒绝跨进程读写虚拟内存成功
	win10中任务管理器进程tab页拒绝结束进程无效
	win10中任务管理器详细信息tab页拒绝结束进程成功

	win10中加载驱动后，任务管理器打不开，ce打不开
	explore.exe进程一直在OpenProcess