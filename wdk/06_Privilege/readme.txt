编译时候需要加上选项,配置如下
项目 -- 属性 -- 链接器 -- 命令行 -- 其它选项
	/INTEGRITYCHECK 

测试环境:
	win10  1903 (18362.30)

功能:
	给指定进程提权
	
原理:
	回顾ObRegisterCallbacks，函数原型如下
		NTSTATUS ObRegisterCallbacks(
		  [in]  POB_CALLBACK_REGISTRATION CallbackRegistration,
		  [out] PVOID                     *RegistrationHandle
		); 

	结构体其他字段这里不做说明，着重说明Altitude
		typedef struct _OB_CALLBACK_REGISTRATION {
		  USHORT                    Version;
		  USHORT                    OperationRegistrationCount;
		  UNICODE_STRING            Altitude;
		  PVOID                     RegistrationContext;
		  OB_OPERATION_REGISTRATION *OperationRegistration;
		} OB_CALLBACK_REGISTRATION, *POB_CALLBACK_REGISTRATION;

	微软的文档:
		https://learn.microsoft.com/en-us/windows-hardware/drivers/ifs/load-order-groups-and-altitudes-for-minifilter-drivers

	按照其说明是不同功能的filter需要使用不同区间的code,并且Altitude的整数值是从大到小的stack排序，执行的顺序经过测试是由大到小执行。
	因此，我们自己的驱动注册的回调，给一个最小的Altitude，这样就会最后执行我们的回调，给最高权限即可
	OperationInformation->Parameters->CreateHandleInformation.DesiredAccess = 0x1fffff

主要代码:
	和注册进程回调基本一直，只是Altitude不同
		RtlInitUnicodeString(&ob1_callback_reg.Altitude, L"123");