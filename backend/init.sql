DROP TABLE IF EXISTS "public"."sys_dept";
DROP TABLE IF EXISTS "public"."sys_role";
DROP TABLE IF EXISTS "public"."sys_role_dept";
DROP TABLE IF EXISTS "public"."sys_permission";
DROP TABLE IF EXISTS "public"."sys_user";
DROP TABLE IF EXISTS "public"."sys_user_role";
DROP TABLE IF EXISTS "public"."sys_module";
DROP TABLE IF EXISTS "public"."sys_action";
DROP TABLE IF EXISTS "public"."sys_action_priority";
DROP TABLE IF EXISTS "public"."sys_menu";
DROP SEQUENCE IF EXISTS "public"."sys_dept_dept_id_seq";
CREATE SEQUENCE "public"."sys_dept_dept_id_seq" 
INCREMENT 1
MINVALUE  1
MAXVALUE 2147483647
START 1
CACHE 1;
DROP SEQUENCE IF EXISTS "public"."sys_role_role_id_seq";
CREATE SEQUENCE "public"."sys_role_role_id_seq" 
INCREMENT 1
MINVALUE  1
MAXVALUE 2147483647
START 1
CACHE 1;
DROP SEQUENCE IF EXISTS "public"."sys_user_user_id_seq";
CREATE SEQUENCE "public"."sys_user_user_id_seq" 
INCREMENT 1
MINVALUE  1
MAXVALUE 2147483647
START 1
CACHE 1;
DROP SEQUENCE IF EXISTS "public"."sys_module_module_id_seq";
CREATE SEQUENCE "public"."sys_module_module_id_seq" 
INCREMENT 1
MINVALUE  1
MAXVALUE 2147483647
START 1
CACHE 1;
DROP SEQUENCE IF EXISTS "public"."sys_action_action_id_seq";
CREATE SEQUENCE "public"."sys_action_action_id_seq" 
INCREMENT 1
MINVALUE  1
MAXVALUE 2147483647
START 1
CACHE 1;
DROP SEQUENCE IF EXISTS "public"."sys_menu_menu_id_seq";
CREATE SEQUENCE "public"."sys_menu_menu_id_seq" 
INCREMENT 1
MINVALUE  1
MAXVALUE 2147483647
START 1
CACHE 1;

CREATE TABLE "public"."sys_dept" (
  "dept_id" int4 NOT NULL DEFAULT nextval('sys_dept_dept_id_seq'::regclass),
  "name" varchar(50) COLLATE "pg_catalog"."default" NOT NULL,
  "sort_num" int4 NOT NULL,
  "parent_id" int4,
  "version" int4 NOT NULL DEFAULT 0,
  "created_by" int4 NOT NULL,
  "created_time" timestamp(6) NOT NULL,
  "updated_by" int4 NOT NULL,
  "updated_time" timestamp(6) NOT NULL,
  "deleted_by" int4,
  "deleted_time" timestamp(6)
)
;
COMMENT ON COLUMN "public"."sys_dept"."name" IS '部门名称';
COMMENT ON COLUMN "public"."sys_dept"."sort_num" IS '部门排序';
COMMENT ON COLUMN "public"."sys_dept"."parent_id" IS '父部门id';
COMMENT ON COLUMN "public"."sys_dept"."version" IS '乐观锁版本号';
COMMENT ON COLUMN "public"."sys_dept"."created_by" IS '创建者';
COMMENT ON COLUMN "public"."sys_dept"."created_time" IS '创建时间';
COMMENT ON COLUMN "public"."sys_dept"."updated_by" IS '最新一次更新者';
COMMENT ON COLUMN "public"."sys_dept"."updated_time" IS '最新一次更新时间';
COMMENT ON COLUMN "public"."sys_dept"."deleted_by" IS '删除者';
COMMENT ON COLUMN "public"."sys_dept"."deleted_time" IS '删除时间';


CREATE TABLE "public"."sys_role" (
  "role_id" int4 NOT NULL DEFAULT nextval('sys_role_role_id_seq'::regclass),
  "name" varchar(255) COLLATE "pg_catalog"."default" NOT NULL,
  "code" varchar(255) COLLATE "pg_catalog"."default" NOT NULL,
  "description" varchar(255) COLLATE "pg_catalog"."default",
  "quota_type" int2 NOT NULL,
  "user_quota" int4,
  "relation_type" int2 NOT NULL,
  "version" int4 NOT NULL DEFAULT 0,
  "created_by" int4 NOT NULL,
  "created_time" timestamp(0) NOT NULL,
  "updated_by" int4 NOT NULL,
  "updated_time" timestamp(0) NOT NULL,
  "deleted_by" int4,
  "deleted_time" timestamp(0)
)
;
COMMENT ON COLUMN "public"."sys_role"."role_id" IS '角色id';
COMMENT ON COLUMN "public"."sys_role"."name" IS '角色名字';
COMMENT ON COLUMN "public"."sys_role"."code" IS '角色代码';
COMMENT ON COLUMN "public"."sys_role"."description" IS '角色描述';
COMMENT ON COLUMN "public"."sys_role"."quota_type" IS '用户数量限制类型 0-不限制 1-总数量限制 2-每个部门用户数量限制';
COMMENT ON COLUMN "public"."sys_role"."user_quota" IS '用户数量限制';
COMMENT ON COLUMN "public"."sys_role"."relation_type" IS '和部门的关联关系 0-所有部门可用 1-指定部门可用 2-指定部门不可用';
COMMENT ON COLUMN "public"."sys_dept"."version" IS '乐观锁版本号';
COMMENT ON COLUMN "public"."sys_role"."created_by" IS '创建者';
COMMENT ON COLUMN "public"."sys_role"."created_time" IS '创建时间';
COMMENT ON COLUMN "public"."sys_role"."updated_by" IS '更新者';
COMMENT ON COLUMN "public"."sys_role"."updated_time" IS '更新时间';
COMMENT ON COLUMN "public"."sys_role"."deleted_by" IS '删除者';
COMMENT ON COLUMN "public"."sys_role"."deleted_time" IS '删除时间';

CREATE TABLE "public"."sys_role_dept" (
  "role_id" int4 NOT NULL,
  "dept_id" int4 NOT NULL,
  "created_by" int4 NOT NULL,
  "created_time" timestamp(6) NOT NULL
)
;
COMMENT ON COLUMN "public"."sys_role_dept"."role_id" IS '角色id';
COMMENT ON COLUMN "public"."sys_role_dept"."dept_id" IS '部门id';
COMMENT ON COLUMN "public"."sys_role_dept"."created_by" IS '创建者';
COMMENT ON COLUMN "public"."sys_role_dept"."created_time" IS '创建时间';

CREATE TABLE "public"."sys_permission" (
  "role_id" int4 NOT NULL,
  "action_id" int4 NOT NULL,
  "scope" int2 NOT NULL,
  "created_by" int4 NOT NULL,
  "created_time" timestamp(6) NOT NULL,
  "updated_by" int4 NOT NULL,
  "updated_time" timestamp(0) NOT NULL,
  "deleted_by" int4,
  "deleted_time" timestamp(0)
)
;
COMMENT ON COLUMN "public"."sys_permission"."role_id" IS '角色id';
COMMENT ON COLUMN "public"."sys_permission"."action_id" IS '功能id';
COMMENT ON COLUMN "public"."sys_permission"."scope" IS '权限范围';
COMMENT ON COLUMN "public"."sys_permission"."created_by" IS '创建者';
COMMENT ON COLUMN "public"."sys_permission"."created_time" IS '创建时间';
COMMENT ON COLUMN "public"."sys_permission"."updated_by" IS '更新者';
COMMENT ON COLUMN "public"."sys_permission"."updated_time" IS '更新时间';
COMMENT ON COLUMN "public"."sys_permission"."deleted_by" IS '删除者';
COMMENT ON COLUMN "public"."sys_permission"."deleted_time" IS '删除时间';

CREATE TABLE "public"."sys_user" (
  "user_id" int4 NOT NULL DEFAULT nextval('sys_user_user_id_seq'::regclass),
  "username" varchar(20) COLLATE "pg_catalog"."default" NOT NULL,
  "password" varchar(60) COLLATE "pg_catalog"."default" NOT NULL,
  "nickname" varchar(50) COLLATE "pg_catalog"."default" NOT NULL,
  "avatar" varchar(255) COLLATE "pg_catalog"."default" NOT NULL,
  "sex" int2 NOT NULL DEFAULT 0,
  "dept_id" int4 NOT NULL,
  "phone_number" char(11) COLLATE "pg_catalog"."default",
  "email" varchar(50) COLLATE "pg_catalog"."default",
  "status" int2 NOT NULL DEFAULT 0,
  "version" int4 NOT NULL DEFAULT 0,
  "created_by" int4 NOT NULL,
  "created_time" timestamp(6) NOT NULL,
  "updated_by" int4 NOT NULL,
  "updated_time" timestamp(6) NOT NULL,
  "deleted_by" int4,
  "deleted_time" timestamp(6)
)
;
COMMENT ON COLUMN "public"."sys_user"."username" IS '用户名';
COMMENT ON COLUMN "public"."sys_user"."password" IS '密码';
COMMENT ON COLUMN "public"."sys_user"."nickname" IS '昵称';
COMMENT ON COLUMN "public"."sys_user"."avatar" IS '头像';
COMMENT ON COLUMN "public"."sys_user"."sex" IS '性别 0-保密 1-男 2-女';
COMMENT ON COLUMN "public"."sys_user"."dept_id" IS '所属部门id';
COMMENT ON COLUMN "public"."sys_user"."phone_number" IS '电话号码';
COMMENT ON COLUMN "public"."sys_user"."email" IS '邮箱';
COMMENT ON COLUMN "public"."sys_user"."status" IS '状态 0-正常 1-禁用';
COMMENT ON COLUMN "public"."sys_user"."version" IS '乐观锁版本号';
COMMENT ON COLUMN "public"."sys_user"."created_by" IS '创建者';
COMMENT ON COLUMN "public"."sys_user"."created_time" IS '创建时间';
COMMENT ON COLUMN "public"."sys_user"."updated_by" IS '最后一次更新者';
COMMENT ON COLUMN "public"."sys_user"."updated_time" IS '最后一次更新时间';
COMMENT ON COLUMN "public"."sys_user"."deleted_by" IS '删除者';
COMMENT ON COLUMN "public"."sys_user"."deleted_time" IS '删除时间';

CREATE TABLE "public"."sys_user_role" (
  "user_id" int4 NOT NULL,
  "role_id" int4 NOT NULL,
  "created_by" int4 NOT NULL,
  "created_time" timestamp(6) NOT NULL
)
;
COMMENT ON COLUMN "public"."sys_user_role"."user_id" IS '用户id';
COMMENT ON COLUMN "public"."sys_user_role"."role_id" IS '角色id';
COMMENT ON COLUMN "public"."sys_user_role"."created_by" IS '创建者';
COMMENT ON COLUMN "public"."sys_user_role"."created_time" IS '创建时间';

CREATE TABLE "public"."sys_module" (
  "module_id" int4 NOT NULL DEFAULT nextval('sys_module_module_id_seq'::regclass),
  "name" varchar(50) COLLATE "pg_catalog"."default" NOT NULL,
  "description" varchar(255) COLLATE "pg_catalog"."default",
  "sort_num" int4 NOT NULL,
  "parent_id" int4,
  "created_by" int4 NOT NULL,
  "created_time" timestamp(6) NOT NULL,
  "updated_by" int4 NOT NULL,
  "updated_time" timestamp(6) NOT NULL,
  "deleted_by" int4,
  "deleted_time" timestamp(6)
)
;
COMMENT ON COLUMN "public"."sys_module"."module_id" IS '模块id';
COMMENT ON COLUMN "public"."sys_module"."name" IS '模块名称';
COMMENT ON COLUMN "public"."sys_module"."description" IS '模块描述';
COMMENT ON COLUMN "public"."sys_module"."sort_num" IS '模块排序';
COMMENT ON COLUMN "public"."sys_module"."parent_id" IS '父模块id';
COMMENT ON COLUMN "public"."sys_module"."created_by" IS '创建者';
COMMENT ON COLUMN "public"."sys_module"."created_time" IS '创建时间';
COMMENT ON COLUMN "public"."sys_module"."updated_by" IS '最新一次更新者';
COMMENT ON COLUMN "public"."sys_module"."updated_time" IS '最新一次更新时间';
COMMENT ON COLUMN "public"."sys_module"."deleted_by" IS '删除者';
COMMENT ON COLUMN "public"."sys_module"."deleted_time" IS '删除时间';

CREATE TABLE "public"."sys_action" (
  "action_id" int4 NOT NULL DEFAULT nextval('sys_action_action_id_seq'::regclass),
  "name" varchar(50) COLLATE "pg_catalog"."default" NOT NULL,
  "code" varchar(50) COLLATE "pg_catalog"."default" NOT NULL,
  "description" varchar(255) COLLATE "pg_catalog"."default",
  "sort_num" int4 NOT NULL,
  "has_data_permission" bool NOT NULL DEFAULT false,
  "module_id" int4 NOT NULL,
  "created_by" int4 NOT NULL,
  "created_time" timestamp(6) NOT NULL,
  "updated_by" int4 NOT NULL,
  "updated_time" timestamp(6) NOT NULL,
  "deleted_by" int4,
  "deleted_time" timestamp(6)
)
;
COMMENT ON COLUMN "public"."sys_action"."action_id" IS '功能id';
COMMENT ON COLUMN "public"."sys_action"."name" IS '功能名称';
COMMENT ON COLUMN "public"."sys_action"."code" IS '功能代码';
COMMENT ON COLUMN "public"."sys_action"."description" IS '功能描述';
COMMENT ON COLUMN "public"."sys_action"."sort_num" IS '功能排序';
COMMENT ON COLUMN "public"."sys_action"."has_data_permission" IS '是否有数据权限';
COMMENT ON COLUMN "public"."sys_action"."module_id" IS '所属模块id';
COMMENT ON COLUMN "public"."sys_action"."created_by" IS '创建者';
COMMENT ON COLUMN "public"."sys_action"."created_time" IS '创建时间';
COMMENT ON COLUMN "public"."sys_action"."updated_by" IS '最新一次更新者';
COMMENT ON COLUMN "public"."sys_action"."updated_time" IS '最新一次更新时间';
COMMENT ON COLUMN "public"."sys_action"."deleted_by" IS '删除者';
COMMENT ON COLUMN "public"."sys_action"."deleted_time" IS '删除时间';

CREATE TABLE "public"."sys_action_priority" (
  "high_id" int4 NOT NULL,
  "low_id" int4 NOT NULL,
  "module_id" int4 NOT NULL,
  "created_by" int4 NOT NULL,
  "created_time" timestamp(6) NOT NULL
)
;
COMMENT ON COLUMN "public"."sys_action_priority"."high_id" IS '高优先级功能id';
COMMENT ON COLUMN "public"."sys_action_priority"."low_id" IS '低优先级功能id';
COMMENT ON COLUMN "public"."sys_action_priority"."module_id" IS '所属模块id';
COMMENT ON COLUMN "public"."sys_action_priority"."created_by" IS '创建者';
COMMENT ON COLUMN "public"."sys_action_priority"."created_time" IS '创建时间';

CREATE TABLE "public"."sys_menu" (
  "menu_id" int4 NOT NULL DEFAULT nextval('sys_menu_menu_id_seq'::regclass),
  "action_id" int4,
  "parent_id" int4,
  "path" varchar(50) COLLATE "pg_catalog"."default",
  "icon" varchar(50) COLLATE "pg_catalog"."default",
  "name" varchar(50) COLLATE "pg_catalog"."default" NOT NULL,
  "sort_num" int4 NOT NULL,
  "component" varchar(50) COLLATE "pg_catalog"."default",
  "type" int2 NOT NULL,
  "created_by" int4 NOT NULL,
  "created_time" timestamp(6) NOT NULL,
  "updated_by" int4 NOT NULL,
  "updated_time" timestamp(6) NOT NULL,
  "deleted_by" int4,
  "deleted_time" timestamp(6)
)
;
COMMENT ON COLUMN "public"."sys_menu"."menu_id" IS '菜单id';
COMMENT ON COLUMN "public"."sys_menu"."action_id" IS '功能id';
COMMENT ON COLUMN "public"."sys_menu"."parent_id" IS '父菜单id';
COMMENT ON COLUMN "public"."sys_menu"."path" IS '路径';
COMMENT ON COLUMN "public"."sys_menu"."icon" IS '图标';
COMMENT ON COLUMN "public"."sys_menu"."name" IS '菜单名称';
COMMENT ON COLUMN "public"."sys_menu"."sort_num" IS '菜单排序';
COMMENT ON COLUMN "public"."sys_menu"."component" IS '组件';
COMMENT ON COLUMN "public"."sys_menu"."type" IS '菜单类型';
COMMENT ON COLUMN "public"."sys_menu"."created_by" IS '创建者';
COMMENT ON COLUMN "public"."sys_menu"."created_time" IS '创建时间';
COMMENT ON COLUMN "public"."sys_menu"."updated_by" IS '最新一次更新者';
COMMENT ON COLUMN "public"."sys_menu"."updated_time" IS '最新一次更新时间';
COMMENT ON COLUMN "public"."sys_menu"."deleted_by" IS '删除者';
COMMENT ON COLUMN "public"."sys_menu"."deleted_time" IS '删除时间';

-- 部门表数据
INSERT INTO "public"."sys_dept" VALUES (1, '钱途无量有限公司', 0, NULL, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (2, '人事部', 0, 1, 0, 1, '2026-01-10 21:48:02', 1, '2026-01-10 21:48:02', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (3, '财务部', 1, 1, 0, 1, '2026-01-10 21:48:27', 1, '2026-01-10 21:48:27', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (4, '技术部', 2, 1, 0, 1, '2026-01-10 21:48:48', 1, '2026-01-10 21:48:48', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (5, 'xx项目开发组', 0, 4, 0, 1, '2026-01-10 21:49:11', 1, '2026-01-10 21:49:11', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (6, 'yy项目开发组', 1, 4, 0, 1, '2026-01-10 21:49:34', 1, '2026-01-10 21:49:34', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (7, '分公司', 1, NULL, 0, 1, '2026-01-10 21:49:55', 1, '2026-01-10 21:49:55', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (8, '财务部', 0, 7, 0, 1, '2026-01-10 21:50:15', 1, '2026-01-10 21:50:15', NULL, NULL);

-- 角色表数据
INSERT INTO "public"."sys_role" VALUES (1, '系统管理员', 'admin', NULL, 1, 3, 2, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (2, '测试角色', 'test', 'test_test_test', 2, 1, 0, 0, 1, '2026-03-29 00:00:00', 1, '2026-03-29 00:00:00', NULL, NULL);

-- 角色部门表数据
INSERT INTO "public"."sys_role_dept" VALUES (1, 7, 1, '2026-04-02 21:53:40.210895');
INSERT INTO "public"."sys_role_dept" VALUES (1, 8, 1, '2026-04-02 21:53:40.210895');

-- 用户表数据
INSERT INTO "public"."sys_user" VALUES (1, 'admin123', '$2a$10$MuPdGRLkS0VxgmPQFHfEVOWgD5wbqpftQM1zHqzTSbHZdCI8i7BUe', '超级管理员', '#', 1, 1, NULL, NULL, 0, 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);

-- 用户角色表数据
INSERT INTO "public"."sys_user_role" VALUES (1, 1, 1, '2026-04-03 22:28:44');

-- 模块表数据
INSERT INTO public.sys_module VALUES (1, '系统管理', null, 0, null, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_module VALUES (2, '组织架构', null, 0, 1, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_module VALUES (3, '部门管理', null, 0, 2, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_module VALUES (4, '用户管理', null, 1, 2, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_module VALUES (5, '权限管理', null, 1, 1, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_module VALUES (6, '角色管理', null, 0, 5, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_module VALUES (7, '权限分配', null, 1, 5, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);

-- 功能表数据
INSERT INTO public.sys_action VALUES (1, '浏览部门管理页面', 'dept:view', null, 0, false, 3, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (2, '查询部门树', 'dept:query', null, 1, true, 3, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (3, '新增部门', 'dept:create', null, 2, true, 3, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (4, '排序部门', 'dept:sort', null, 3, true, 3, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (5, '更新部门', 'dept:update', null, 4, true, 3, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (6, '删除部门', 'dept:delete', null, 5, true, 3, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (7, '浏览用户管理页面', 'user:view', null, 0, false, 4, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (8, '查询用户', 'user:query', null, 1, true, 4, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (9, '新增用户', 'user:create', null, 2, true, 4, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (10, '更新用户', 'user:update', null, 3, true, 4, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (11, '删除用户', 'user:delete', null, 4, true, 4, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (12, '浏览角色管理页面', 'role:view', null, 0, false, 6, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (13, '查询角色', 'role:query', null, 1, false, 6, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (14, '新增角色', 'role:create', null, 2, false, 6, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (15, '更新角色', 'role:update', null, 3, false, 6, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);
INSERT INTO public.sys_action VALUES (16, '删除角色', 'role:delete', null, 4, false, 6, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', null, null);

--- 优先级表数据
INSERT INTO public.sys_action_priority VALUES (2, 3, 3, 1, '2026-07-22 15:32:58.000000');
INSERT INTO public.sys_action_priority VALUES (2, 4, 3, 1, '2026-07-22 15:32:58.000000');
INSERT INTO public.sys_action_priority VALUES (2, 5, 3, 1, '2026-07-22 15:32:58.000000');
INSERT INTO public.sys_action_priority VALUES (5, 6, 3, 1, '2026-07-22 15:32:58.000000');
INSERT INTO public.sys_action_priority VALUES (7, 8, 4, 1, '2026-07-22 15:32:58.000000');
INSERT INTO public.sys_action_priority VALUES (8, 9, 4, 1, '2026-07-22 15:32:58.000000');
INSERT INTO public.sys_action_priority VALUES (8, 10, 4, 1, '2026-07-22 15:32:58.000000');
INSERT INTO public.sys_action_priority VALUES (10, 11, 4, 1, '2026-07-22 15:32:58.000000');
INSERT INTO public.sys_action_priority VALUES (12, 13, 6, 1, '2026-07-22 15:32:58.000000');
INSERT INTO public.sys_action_priority VALUES (13, 14, 6, 1, '2026-07-22 15:32:58.000000');
INSERT INTO public.sys_action_priority VALUES (13, 15, 6, 1, '2026-07-22 15:32:58.000000');
INSERT INTO public.sys_action_priority VALUES (15, 16, 6, 1, '2026-07-22 15:32:58.000000');

ALTER SEQUENCE "public"."sys_dept_dept_id_seq"
OWNED BY "public"."sys_dept"."dept_id";
SELECT setval('"public"."sys_dept_dept_id_seq"', 8, true);
ALTER SEQUENCE "public"."sys_role_role_id_seq"
OWNED BY "public"."sys_role"."role_id";
SELECT setval('"public"."sys_role_role_id_seq"', 2, true);
ALTER SEQUENCE "public"."sys_user_user_id_seq"
OWNED BY "public"."sys_user"."user_id";
SELECT setval('"public"."sys_user_user_id_seq"', 1, true);
ALTER SEQUENCE "public"."sys_module_module_id_seq"
OWNED BY "public"."sys_module"."module_id";
SELECT setval('"public"."sys_module_module_id_seq"', 7, true);
ALTER SEQUENCE "public"."sys_action_action_id_seq"
OWNED BY "public"."sys_action"."action_id";
SELECT setval('"public"."sys_action_action_id_seq"', 16, true);
ALTER SEQUENCE "public"."sys_menu_menu_id_seq"
OWNED BY "public"."sys_menu"."menu_id";
SELECT setval('"public"."sys_menu_menu_id_seq"', 1, false);

ALTER TABLE "public"."sys_dept" ADD CONSTRAINT "sys_dept_pkey" PRIMARY KEY ("dept_id");
ALTER TABLE "public"."sys_role" ADD CONSTRAINT "sys_role_pkey" PRIMARY KEY ("role_id");
ALTER TABLE "public"."sys_role_dept" ADD CONSTRAINT "sys_role_dept_pkey" PRIMARY KEY ("role_id", "dept_id");
ALTER TABLE "public"."sys_permission" ADD CONSTRAINT "sys_permission_pkey" PRIMARY KEY ("role_id", "action_id");
ALTER TABLE "public"."sys_user" ADD CONSTRAINT "sys_user_pkey" PRIMARY KEY ("user_id");
ALTER TABLE "public"."sys_user_role" ADD CONSTRAINT "sys_user_role_pkey" PRIMARY KEY ("user_id", "role_id");
ALTER TABLE "public"."sys_module" ADD CONSTRAINT "sys_module_pkey" PRIMARY KEY ("module_id");
ALTER TABLE "public"."sys_action" ADD CONSTRAINT "sys_action_pkey" PRIMARY KEY ("action_id");
ALTER TABLE "public"."sys_action_priority" ADD CONSTRAINT "sys_action_priority_pkey" PRIMARY KEY ("high_id", "low_id");
ALTER TABLE "public"."sys_menu" ADD CONSTRAINT "sys_menu_pkey" PRIMARY KEY ("menu_id");
