/*
 Navicat Premium Data Transfer

 Source Server         : drogonAdmin
 Source Server Type    : PostgreSQL
 Source Server Version : 180001
 Source Host           : localhost:5432
 Source Catalog        : drogon_admin_db
 Source Schema         : public

 Target Server Type    : PostgreSQL
 Target Server Version : 180001
 File Encoding         : 65001

 Date: 29/03/2026 17:40:52
*/

-- ----------------------------
-- Table structure for sys_dept
-- ----------------------------
DROP TABLE IF EXISTS "public"."sys_dept";

-- ----------------------------
-- Table structure for sys_role
-- ----------------------------
DROP TABLE IF EXISTS "public"."sys_role";

-- ----------------------------
-- Table structure for sys_role_dept
-- ----------------------------
DROP TABLE IF EXISTS "public"."sys_role_dept";

-- ----------------------------
-- Table structure for sys_user
-- ----------------------------
DROP TABLE IF EXISTS "public"."sys_user";

-- ----------------------------
-- Sequence structure for sys_dept_dept_id_seq
-- ----------------------------
DROP SEQUENCE IF EXISTS "public"."sys_dept_dept_id_seq";
CREATE SEQUENCE "public"."sys_dept_dept_id_seq" 
INCREMENT 1
MINVALUE  1
MAXVALUE 2147483647
START 1
CACHE 1;

-- ----------------------------
-- Sequence structure for sys_role_dept_id_seq
-- ----------------------------
DROP SEQUENCE IF EXISTS "public"."sys_role_dept_id_seq";
CREATE SEQUENCE "public"."sys_role_dept_id_seq" 
INCREMENT 1
MINVALUE  1
MAXVALUE 2147483647
START 1
CACHE 1;

-- ----------------------------
-- Sequence structure for sys_role_role_id_seq
-- ----------------------------
DROP SEQUENCE IF EXISTS "public"."sys_role_role_id_seq";
CREATE SEQUENCE "public"."sys_role_role_id_seq" 
INCREMENT 1
MINVALUE  1
MAXVALUE 2147483647
START 1
CACHE 1;

-- ----------------------------
-- Table structure for sys_dept
-- ----------------------------
CREATE TABLE "public"."sys_dept" (
  "dept_id" int4 NOT NULL DEFAULT nextval('sys_dept_dept_id_seq'::regclass),
  "name" varchar(50) COLLATE "pg_catalog"."default" NOT NULL,
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
COMMENT ON COLUMN "public"."sys_dept"."name" IS '部门名称';
COMMENT ON COLUMN "public"."sys_dept"."sort_num" IS '部门排序';
COMMENT ON COLUMN "public"."sys_dept"."parent_id" IS '父部门id';
COMMENT ON COLUMN "public"."sys_dept"."created_by" IS '创建者';
COMMENT ON COLUMN "public"."sys_dept"."created_time" IS '创建时间';
COMMENT ON COLUMN "public"."sys_dept"."updated_by" IS '最新一次更新者';
COMMENT ON COLUMN "public"."sys_dept"."updated_time" IS '最新一次更新时间';
COMMENT ON COLUMN "public"."sys_dept"."deleted_by" IS '删除者';
COMMENT ON COLUMN "public"."sys_dept"."deleted_time" IS '删除时间';

-- ----------------------------
-- Records of sys_dept
-- ----------------------------
INSERT INTO "public"."sys_dept" VALUES (1, '钱途无量有限公司', 0, NULL, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (2, '人事部', 0, 1, 1, '2026-01-10 21:48:02', 1, '2026-01-10 21:48:02', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (3, '财务部', 1, 1, 1, '2026-01-10 21:48:27', 1, '2026-01-10 21:48:27', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (4, '技术部', 2, 1, 1, '2026-01-10 21:48:48', 1, '2026-01-10 21:48:48', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (5, 'xx项目开发组', 0, 4, 1, '2026-01-10 21:49:11', 1, '2026-01-10 21:49:11', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (6, 'yy项目开发组', 1, 4, 1, '2026-01-10 21:49:34', 1, '2026-01-10 21:49:34', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (7, '分公司', 1, NULL, 1, '2026-01-10 21:49:55', 1, '2026-01-10 21:49:55', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (8, '财务部', 0, 7, 1, '2026-01-10 21:50:15', 1, '2026-01-10 21:50:15', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (9, '宣传部', 3, 1, 1, '2026-01-15 22:34:35.377476', 1, '2026-01-15 22:35:05.84148', 1, '2026-01-21 17:07:02.365522');

-- ----------------------------
-- Table structure for sys_role
-- ----------------------------
CREATE TABLE "public"."sys_role" (
  "role_id" int4 NOT NULL DEFAULT nextval('sys_role_role_id_seq'::regclass),
  "name" varchar(255) COLLATE "pg_catalog"."default" NOT NULL,
  "code" varchar(255) COLLATE "pg_catalog"."default" NOT NULL,
  "description" varchar(255) COLLATE "pg_catalog"."default",
  "quota_type" int2 NOT NULL,
  "user_quota" int4,
  "relation_type" int2 NOT NULL,
  "created_by" int4 NOT NULL,
  "created_time" date NOT NULL,
  "updated_by" int4 NOT NULL,
  "updated_time" date NOT NULL,
  "deleted_by" int4,
  "deleted_time" date
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

-- ----------------------------
-- Records of sys_role
-- ----------------------------

-- ----------------------------
-- Table structure for sys_role_dept
-- ----------------------------
CREATE TABLE "public"."sys_role_dept" (
  "id" int4 NOT NULL DEFAULT nextval('sys_role_dept_id_seq'::regclass),
  "role_id" int4 NOT NULL,
  "dept_id" int4 NOT NULL,
  "created_by" int4 NOT NULL,
  "created_time" timestamp(6) NOT NULL,
  "updated_by" int4 NOT NULL,
  "updated_time" timestamp(6) NOT NULL,
  "deleted_by" int4,
  "deleted_time" timestamp(6)
)
;
COMMENT ON COLUMN "public"."sys_role_dept"."role_id" IS '角色id';
COMMENT ON COLUMN "public"."sys_role_dept"."dept_id" IS '部门id';
COMMENT ON COLUMN "public"."sys_role_dept"."created_by" IS '创建者';
COMMENT ON COLUMN "public"."sys_role_dept"."created_time" IS '创建时间';
COMMENT ON COLUMN "public"."sys_role_dept"."updated_by" IS '最新一次更新者';
COMMENT ON COLUMN "public"."sys_role_dept"."updated_time" IS '最新一次更新时间';
COMMENT ON COLUMN "public"."sys_role_dept"."deleted_by" IS '删除者';
COMMENT ON COLUMN "public"."sys_role_dept"."deleted_time" IS '删除时间';

-- ----------------------------
-- Records of sys_role_dept
-- ----------------------------

-- ----------------------------
-- Table structure for sys_user
-- ----------------------------
CREATE TABLE "public"."sys_user" (
  "user_id" int4 NOT NULL,
  "username" varchar(20) COLLATE "pg_catalog"."default" NOT NULL,
  "password" varchar(20) COLLATE "pg_catalog"."default" NOT NULL,
  "nickname" varchar(50) COLLATE "pg_catalog"."default" NOT NULL,
  "avatar" varchar(255) COLLATE "pg_catalog"."default" NOT NULL,
  "sex" int2 NOT NULL DEFAULT 0,
  "dept_id" int4 NOT NULL,
  "phone_number" char(11) COLLATE "pg_catalog"."default",
  "email" varchar(50) COLLATE "pg_catalog"."default",
  "status" int2 NOT NULL DEFAULT 0,
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
COMMENT ON COLUMN "public"."sys_user"."created_by" IS '创建者';
COMMENT ON COLUMN "public"."sys_user"."created_time" IS '创建时间';
COMMENT ON COLUMN "public"."sys_user"."updated_by" IS '最后一次更新者';
COMMENT ON COLUMN "public"."sys_user"."updated_time" IS '最后一次更新时间';
COMMENT ON COLUMN "public"."sys_user"."deleted_by" IS '删除者';
COMMENT ON COLUMN "public"."sys_user"."deleted_time" IS '删除时间';

-- ----------------------------
-- Records of sys_user
-- ----------------------------

-- ----------------------------
-- Alter sequences owned by
-- ----------------------------
ALTER SEQUENCE "public"."sys_dept_dept_id_seq"
OWNED BY "public"."sys_dept"."dept_id";
SELECT setval('"public"."sys_dept_dept_id_seq"', 9, true);

-- ----------------------------
-- Primary Key structure for table sys_dept
-- ----------------------------
ALTER TABLE "public"."sys_dept" ADD CONSTRAINT "sys_dept_pkey" PRIMARY KEY ("dept_id");

-- ----------------------------
-- Primary Key structure for table sys_role
-- ----------------------------
ALTER TABLE "public"."sys_role" ADD CONSTRAINT "sys_role_pkey" PRIMARY KEY ("role_id");

-- ----------------------------
-- Indexes structure for table sys_role_dept
-- ----------------------------
CREATE UNIQUE INDEX "uk_sys_role_dept_role_dept_undelete" ON "public"."sys_role_dept" USING btree (
  "role_id" "pg_catalog"."int4_ops" ASC NULLS LAST,
  "dept_id" "pg_catalog"."int4_ops" ASC NULLS LAST
) WHERE deleted_by IS NULL;

-- ----------------------------
-- Primary Key structure for table sys_role_dept
-- ----------------------------
ALTER TABLE "public"."sys_role_dept" ADD CONSTRAINT "sys_role_dept_pkey" PRIMARY KEY ("id");

-- ----------------------------
-- Primary Key structure for table sys_user
-- ----------------------------
ALTER TABLE "public"."sys_user" ADD CONSTRAINT "sys_user_pkey" PRIMARY KEY ("user_id");
