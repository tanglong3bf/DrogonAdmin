DROP TABLE IF EXISTS "public"."sys_role";
DROP TABLE IF EXISTS "public"."sys_permission";
DROP TABLE IF EXISTS "public"."sys_module";
DROP TABLE IF EXISTS "public"."sys_action";
DROP TABLE IF EXISTS "public"."sys_menu";

DROP SEQUENCE IF EXISTS "public"."sys_role_role_id_seq";
CREATE SEQUENCE "public"."sys_role_role_id_seq" 
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

CREATE TABLE "public"."sys_role" (
  "role_id" int4 NOT NULL DEFAULT nextval('sys_role_role_id_seq'::regclass),
  "name" varchar(255) COLLATE "pg_catalog"."default" NOT NULL,
  "code" varchar(255) COLLATE "pg_catalog"."default" NOT NULL,
  "description" varchar(255) COLLATE "pg_catalog"."default",
  "quota_type" int2 NOT NULL,
  "user_quota" int4,
  "relation_type" int2 NOT NULL,
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
COMMENT ON COLUMN "public"."sys_role"."created_by" IS '创建者';
COMMENT ON COLUMN "public"."sys_role"."created_time" IS '创建时间';
COMMENT ON COLUMN "public"."sys_role"."updated_by" IS '更新者';
COMMENT ON COLUMN "public"."sys_role"."updated_time" IS '更新时间';
COMMENT ON COLUMN "public"."sys_role"."deleted_by" IS '删除者';
COMMENT ON COLUMN "public"."sys_role"."deleted_time" IS '删除时间';

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
COMMENT ON COLUMN "public"."sys_action"."module_id" IS '所属模块id';
COMMENT ON COLUMN "public"."sys_action"."created_by" IS '创建者';
COMMENT ON COLUMN "public"."sys_action"."created_time" IS '创建时间';
COMMENT ON COLUMN "public"."sys_action"."updated_by" IS '最新一次更新者';
COMMENT ON COLUMN "public"."sys_action"."updated_time" IS '最新一次更新时间';
COMMENT ON COLUMN "public"."sys_action"."deleted_by" IS '删除者';
COMMENT ON COLUMN "public"."sys_action"."deleted_time" IS '删除时间';

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

-- 角色表数据
INSERT INTO public.sys_role VALUES (1, '超级管理员', 'admin', '分配了user:*权限', 0, NULL, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);

-- 权限表数据
INSERT INTO public.sys_permission VALUES (1, 3, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);
INSERT INTO public.sys_permission VALUES (1, 4, 0, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);

-- 模块表数据
INSERT INTO public.sys_module VALUES (1, '模块-1', '已经被软删除的模块', 0, NULL, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000');
INSERT INTO public.sys_module VALUES (2, '模块-2', '有子模块的模块', 1, NULL, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);
INSERT INTO public.sys_module VALUES (3, '模块-3', '占位子模块', 0, 2, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);
INSERT INTO public.sys_module VALUES (4, '模块-4', '有功能的模块', 2, NULL, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);
INSERT INTO public.sys_module VALUES (5, '模块-5', '功能被分配了权限', 3, NULL, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);
INSERT INTO public.sys_module VALUES (6, '模块-6', '功能被菜单使用', 3, NULL, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);

-- 功能表数据
INSERT INTO public.sys_action VALUES (1, '浏览部门管理页面', 'dept:view', NULL, 0, 4, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);
INSERT INTO public.sys_action VALUES (2, '新增部门', 'dept:create', NULL, 1, 4, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);
INSERT INTO public.sys_action VALUES (3, '浏览用户管理页面', 'user:view', NULL, 0, 5, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);
INSERT INTO public.sys_action VALUES (4, '新增用户', 'user:create', NULL, 1, 5, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);
INSERT INTO public.sys_action VALUES (5, '浏览角色管理页面', 'role:view', '被菜单使用', 0, 6, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);

-- 菜单表数据
INSERT INTO public.sys_menu VALUES (1, 5, NULL, '/role', NULL, '角色管理', 0, '/path', 1, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);

ALTER SEQUENCE "public"."sys_role_role_id_seq"
OWNED BY "public"."sys_role"."role_id";
SELECT setval('"public"."sys_role_role_id_seq"', 1, true);
ALTER SEQUENCE "public"."sys_module_module_id_seq"
OWNED BY "public"."sys_module"."module_id";
SELECT setval('"public"."sys_module_module_id_seq"', 6, true);
ALTER SEQUENCE "public"."sys_action_action_id_seq"
OWNED BY "public"."sys_action"."action_id";
SELECT setval('"public"."sys_action_action_id_seq"', 5, true);
ALTER SEQUENCE "public"."sys_menu_menu_id_seq"
OWNED BY "public"."sys_menu"."menu_id";
SELECT setval('"public"."sys_menu_menu_id_seq"', 1, true);

ALTER TABLE "public"."sys_role" ADD CONSTRAINT "sys_role_pkey" PRIMARY KEY ("role_id");
ALTER TABLE "public"."sys_permission" ADD CONSTRAINT "sys_permission_pkey" PRIMARY KEY ("role_id", "action_id");
ALTER TABLE "public"."sys_module" ADD CONSTRAINT "sys_module_pkey" PRIMARY KEY ("module_id");
ALTER TABLE "public"."sys_action" ADD CONSTRAINT "sys_action_pkey" PRIMARY KEY ("action_id");
ALTER TABLE "public"."sys_menu" ADD CONSTRAINT "sys_menu_pkey" PRIMARY KEY ("menu_id");
