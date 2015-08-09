drop database JobSeek
create database JobSeek
--�������ݿ�
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
--�������²�����JobSeek���ݿ���ִ��

create table Industry
--��ҵ��������ҵ����Ϣ
(
	IndustryID int identity(1, 1) constraint Industry_Prim primary key,
--��ҵ���
	Name nvarchar(20) constraint Industry_Name_CONS NOT NULL
--��ҵ����
)

create table Property
--��˾���ʱ����湫˾�ļ�������
(
	PID int identity(1, 1) constraint Property_Prim primary key,
--��˾���ʱ��
	Name nchar(8) constraint Property_Name_CONS NOT NULL
--��˾��������
)

create table Province
(
	PID char(2) constraint Province_Prim primary key,
--ʡ/������/ֱϽ�б�ţ�ʵΪ�õ����֤��ǰ��λ
	Name nvarchar(20) constraint Province_UNIQ unique
--ʡ/������/ֱϽ������
)

create table Region
--����ȫ����ʡ����Ϣ
(
	AreaCode char(4) constraint Region_Prim primary key,
--��������
	Name nvarchar(20),
--��������
	PID char(2) constraint Region_PID_Fore foreign key references Province(PID)
--��������ʡ/������/ֱϽ�б��
)

create table Education
--ѧ����
(
	EID int identity(1, 1) constraint Education_Prim primary key,
--ѧ�����
	Name nchar(10)
--ѧ������
)

create table JobSeekers
--��ְ����Ϣ��
(
	JID int identity(1,1) constraint JobSeekers_Prim primary key,
--��ְ�߱��
	Name nvarchar(30) constraint JobSeekers_Name_CONS NOT NULL,
--��ְ������
	Sex nchar(2) constraint JobSeekers_Sex_CONS NOT NULL default 'Ů',
--��ְ���Ա�
	Age int constraint JobSeekers_Age_CONS NOT NULL,
--��ְ������
	EID int constraint JobSeekers_EID_Fore foreign key references Education(EID),
--��ְ��ѧ��
	Professional nvarchar(50),
--רҵ
	WorkExperience int,
--��������(��λ����)
	AreaCode char(4) constraint JobSeekers_AreaCode_Fore foreign key references Region(AreaCode),
--�����Ĺ�����������
	IndustryID int constraint JobSeekers_IndustryID_Fore foreign key references Industry(IndustryID),
--ϣ�����µ���ҵ���
	Salary money,
--�����нˮ����(��λ��Ԫ)	
	Email char(50)
--����
)

create table Resume
--������
(
	RID int identity(1, 1) constraint Resume_Prim primary key,
--�������
	JID int constraint Resume_JID_Fore foreign key references JobSeekers(JID),
--��ְ�߱��
	FileName varchar(300)
--�����ļ�ȫ��
)

create table EmailInf
--�ʼ����ͽ��ռ�¼��
(
	Email char(50) constraint EmailInf_Prim primary key,
--�����ַ
	LastSendTime datetime
--���һ�η���ʱ��
)

create table Company
--��˾��
(
	CID int identity(1, 1) constraint Company_Prim primary key,
--��˾���
	CN nvarchar(50) constraint Company_CN_UNIQ unique,
--��˾����
	IndustryID int constraint Company_IndustryID_Fore foreign key references Industry(IndustryID),
--��˾������ҵ
	PID int constraint Company_PID_Fore foreign key references Property(PID),
--��˾����
	Population int,
--��˾��ģ(����������)
	AreaCode char(4) constraint Company_AreaCode_Fore foreign key references Region(AreaCode),
--��˾�ܲ����ڵ�������
	Address nvarchar(200),
--��˾�ܲ���ϸ��ַ
	MailCode char(6),
--��˾�ܲ��ʱ�
	PhoneNumber char(15),
--��˾�ܲ���ϵ�绰
	WebSite char(50)
--��˾��ҳ
)


create table Career
--ְλ��
(
	CarID int identity(1, 1) constraint Career_Prim primary key,
--ְλ���
	CarName nvarchar(50),
--ְλ����
	CID int constraint Career_CID_Fore foreign key references Company(CID),
--ְλ������˾���
	AreaCode char(4) constraint Career_AreaCode_Fore foreign key references Region(AreaCode),
--ְλ���ڵ�������
	WorkAddress nvarchar(200),
--ְλ��ϸ��ַ
	Departments nvarchar(50),
--ְλ��������
	WorkDuration int,
--������������(��λ����)
	NeedWorker int,
--��Ƹ����
	ReleaseDate datetime,
--ְλ��Ϣ��������
	EID int constraint Career_EID_Fore foreign key references Education(EID),
--ѧ��Ҫ��
	AgeTop int,
--��������
	AgeBottom int,
--��������
	WorkResponsibilities nvarchar(50),
--����ְ�����
	Email char(50),
--��ְ��ϵ����
	PhoneNumber char(15),
--��ְ��ϵ�绰
	ContactName nvarchar(30)
--��ְ��ϵ�˳ƺ�
)
