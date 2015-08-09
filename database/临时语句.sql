drop database JobSeek
create database JobSeek
--创建数据库
on
(
	name = JobSeek_Data,
	filename = 'C:\JobSeekData.mdf',
	size = 20,
	maxsize = 500,
	filegrowth = 5
)
log on
(
	name = JobSeek_Log,
	filename = 'C:\JobSeekData.ldf',
	size = 5,
	maxsize = 100,
	filegrowth = 5
)

use JobSeek
--声明以下操作在JobSeek数据库中执行

create table Industry
--行业表，储存行业的信息
(
	IndustryID int identity(1, 1) constraint Industry_Prim primary key,
--行业编号
	Name nvarchar(20) constraint Industry_Name_CONS NOT NULL
--行业名称
)

create table Property
--公司性质表，储存公司的几种性质
(
	PID int identity(1, 1) constraint Property_Prim primary key,
--公司性质编号
	Name nchar(8) constraint Property_Name_CONS NOT NULL
--公司性质名称
)

create table Province
(
	PID char(2) constraint Province_Prim primary key,
--省/自治区/直辖市编号，实为该地身份证号前两位
	Name nvarchar(20) constraint Province_UNIQ unique
--省/自治区/直辖市名称
)

create table Region
--储存全国的省市信息
(
	AreaCode char(4) constraint Region_Prim primary key,
--地区区号
	Name nvarchar(20),
--地区名称
	PID char(2) constraint Region_PID_Fore foreign key references Province(PID)
--地区所属省/自治区/直辖市编号
)

create table Education
--学历表
(
	EID int identity(1, 1) constraint Education_Prim primary key,
--学历编号
	Name nchar(10)
--学历名称
)

create table JobSeekers
--求职者信息表
(
	JID int identity(1,1) constraint JobSeekers_Prim primary key,
--求职者编号
	Name nvarchar(30) constraint JobSeekers_Name_CONS NOT NULL,
--求职者姓名
	Sex nchar(2) constraint JobSeekers_Sex_CONS NOT NULL default '女',
--求职者性别
	Age int constraint JobSeekers_Age_CONS NOT NULL,
--求职者年龄
	EID int constraint JobSeekers_EID_Fore foreign key references Education(EID),
--求职者学历
	Professional nvarchar(50),
--专业
	WorkExperience int,
--工作经验(单位：年)
	AreaCode char(4) constraint JobSeekers_AreaCode_Fore foreign key references Region(AreaCode),
--期望的工作地区区号
	IndustryID int constraint JobSeekers_IndustryID_Fore foreign key references Industry(IndustryID),
--希望从事的行业编号
	Salary money,
--允许的薪水下限(单位：元)	
	Email char(50)
--邮箱
)

create table Resume
--简历表
(
	RID int identity(1, 1) constraint Resume_Prim primary key,
--简历编号
	JID int constraint Resume_JID_Fore foreign key references JobSeekers(JID),
--求职者编号
	FileName varchar(300)
--简历文件全名
)

create table EmailInf
--邮件发送接收记录表
(
	Email char(50) constraint EmailInf_Prim primary key,
--邮箱地址
	LastSendTime datetime
--最后一次发送时间
)

create table Company
--公司表
(
	CID int identity(1, 1) constraint Company_Prim primary key,
--公司编号
	CN nvarchar(50) constraint Company_CN_UNIQ unique,
--公司名称
	IndustryID int constraint Company_IndustryID_Fore foreign key references Industry(IndustryID),
--公司所属行业
	PID int constraint Company_PID_Fore foreign key references Property(PID),
--公司性质
	Population int,
--公司规模(多少人以上)
	AreaCode char(4) constraint Company_AreaCode_Fore foreign key references Region(AreaCode),
--公司总部所在地区区号
	Address nvarchar(200),
--公司总部详细地址
	MailCode char(6),
--公司总部邮编
	PhoneNumber char(15),
--公司总部联系电话
	WebSite char(50)
--公司主页
)


create table Career
--职位表
(
	CarID int identity(1, 1) constraint Career_Prim primary key,
--职位编号
	CarName nvarchar(50),
--职位名称
	CID int constraint Career_CID_Fore foreign key references Company(CID),
--职位所属公司编号
	AreaCode char(4) constraint Career_AreaCode_Fore foreign key references Region(AreaCode),
--职位所在地区区号
	WorkAddress nvarchar(200),
--职位详细地址
	Departments nvarchar(50),
--职位所属部门
	WorkDuration int,
--工作年限下限(单位：年)
	NeedWorker int,
--招聘人数
	ReleaseDate datetime,
--职位信息发布日期
	EID int constraint Career_EID_Fore foreign key references Education(EID),
--学历要求
	AgeTop int,
--年龄上限
	AgeBottom int,
--年龄下限
	WorkResponsibilities nvarchar(50),
--工作职责简述
	Email char(50),
--求职联系邮箱
	PhoneNumber char(15),
--求职联系电话
	ContactName nvarchar(30)
--求职联系人称呼
)
