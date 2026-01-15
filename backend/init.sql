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

 Date: 15/01/2026 22:26:45
*/


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
-- Table structure for sys_dept
-- ----------------------------
DROP TABLE IF EXISTS "public"."sys_dept";
CREATE TABLE "public"."sys_dept" (
  "dept_id" int4 NOT NULL DEFAULT nextval('sys_dept_dept_id_seq'::regclass),
  "name" varchar(50) COLLATE "pg_catalog"."default" NOT NULL,
  "order_no" int4 NOT NULL,
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
COMMENT ON COLUMN "public"."sys_dept"."order_no" IS '部门排序';
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
INSERT INTO "public"."sys_dept" VALUES (1, '钱途无量有限公司', 1, NULL, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (2, '人事部', 1, 1, 1, '2026-01-10 21:48:02', 1, '2026-01-10 21:48:02', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (3, '财务部', 2, 1, 1, '2026-01-10 21:48:27', 1, '2026-01-10 21:48:27', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (4, '技术部', 3, 1, 1, '2026-01-10 21:48:48', 1, '2026-01-10 21:48:48', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (5, 'xx项目开发组', 1, 4, 1, '2026-01-10 21:49:11', 1, '2026-01-10 21:49:11', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (6, 'yy项目开发组', 2, 4, 1, '2026-01-10 21:49:34', 1, '2026-01-10 21:49:34', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (7, '分公司', 2, NULL, 1, '2026-01-10 21:49:55', 1, '2026-01-10 21:49:55', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (8, '财务部', 1, 8, 1, '2026-01-10 21:50:15', 1, '2026-01-10 21:50:15', NULL, NULL);

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
