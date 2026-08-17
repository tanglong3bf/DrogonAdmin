DROP TABLE IF EXISTS "public"."sys_dept";
DROP SEQUENCE IF EXISTS "public"."sys_dept_dept_id_seq";
CREATE SEQUENCE "public"."sys_dept_dept_id_seq" 
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

-- 无黑白名单
INSERT INTO "public"."sys_dept" VALUES (1, '部门1', 0, NULL, 0,  1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
-- 有白名单
INSERT INTO "public"."sys_dept" VALUES (2, '部门2', 1, NULL, 0,  1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
-- 有黑名单
INSERT INTO "public"."sys_dept" VALUES (3, '部门3', 2, NULL, 0,  1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
-- 既有白名单也有黑名单
INSERT INTO "public"."sys_dept" VALUES (4, '部门4', 3, NULL, 0,  1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);

ALTER SEQUENCE "public"."sys_dept_dept_id_seq"
OWNED BY "public"."sys_dept"."dept_id";
SELECT setval('"public"."sys_dept_dept_id_seq"', 4, true);
ALTER TABLE "public"."sys_dept" ADD CONSTRAINT "sys_dept_pkey" PRIMARY KEY ("dept_id");

DROP TABLE IF EXISTS "public"."sys_role";
DROP SEQUENCE IF EXISTS "public"."sys_role_role_id_seq";
CREATE SEQUENCE "public"."sys_role_role_id_seq" 
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

INSERT INTO "public"."sys_role" VALUES (1, '管理员-1', 'admin-1', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (2, '管理员-2', 'admin-2', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (3, '管理员-3', 'admin-3', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (4, '管理员-4', 'admin-4', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (5, '管理员-5', 'admin-5', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (6, '管理员-6', 'admin-6', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (7, '管理员-7', 'admin-7', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (8, '管理员-8', 'admin-8', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (9, '管理员-9', 'admin-9', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (10, '管理员-10', 'admin-10', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (11, '管理员-11', 'admin-11', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (12, '管理员-12', 'admin-12', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (13, '管理员-13', 'admin-13', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (14, '管理员-14', 'admin-14', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (15, '管理员-15', 'admin-15', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (16, '管理员-16', 'admin-16', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (17, '管理员-17', 'admin-17', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (18, '管理员-18', 'admin-18', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (19, '管理员-19', 'admin-19', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (20, '管理员-20', 'admin-20', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (21, '管理员-21', 'admin-21', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (22, '管理员-22', 'admin-22', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (23, '管理员-23', 'admin-23', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (24, '管理员-24', 'admin-24', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (25, '管理员-25', 'admin-25', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (26, '管理员-26', 'admin-26', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (27, '管理员-27', 'admin-27', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (28, '管理员-28', 'admin-28', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (29, '管理员-29', 'admin-29', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (30, '管理员-30', 'admin-30', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (31, '管理员-31', 'admin-31', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (32, '管理员-32', 'admin-32', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (33, '管理员-33', 'admin-33', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (34, '管理员-34', 'admin-34', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (35, '管理员-35', 'admin-35', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (36, '管理员-36', 'admin-36', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (37, '管理员-37', 'admin-37', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (38, '管理员-38', 'admin-38', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (39, '管理员-39', 'admin-39', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (40, '管理员-40', 'admin-40', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (41, '管理员-41', 'admin-41', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (42, '管理员-42', 'admin-42', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (43, '管理员-43', 'admin-43', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (44, '管理员-44', 'admin-44', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (45, '管理员-45', 'admin-45', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (46, '管理员-46', 'admin-46', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (47, '管理员-47', 'admin-47', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (48, '管理员-48', 'admin-48', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (49, '管理员-49', 'admin-49', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (50, '管理员-50', 'admin-50', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (51, '管理员-51', 'admin-51', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (52, '管理员-52', 'admin-52', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (53, '管理员-53', 'admin-53', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (54, '管理员-54', 'admin-54', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (55, '管理员-55', 'admin-55', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (56, '管理员-56', 'admin-56', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (57, '管理员-57', 'admin-57', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (58, '管理员-58', 'admin-58', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (59, '管理员-59', 'admin-59', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (60, '管理员-60', 'admin-60', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (61, '管理员-61', 'admin-61', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (62, '管理员-62', 'admin-62', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (63, '管理员-63', 'admin-63', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (64, '管理员-64', 'admin-64', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (65, '管理员-65', 'admin-65', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (66, '管理员-66', 'admin-66', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (67, '管理员-67', 'admin-67', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (68, '管理员-68', 'admin-68', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (69, '管理员-69', 'admin-69', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (70, '管理员-70', 'admin-70', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (71, '管理员-71', 'admin-71', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (72, '管理员-72', 'admin-72', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (73, '管理员-73', 'admin-73', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (74, '管理员-74', 'admin-74', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (75, '管理员-75', 'admin-75', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (76, '管理员-76', 'admin-76', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (77, '管理员-77', 'admin-77', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (78, '管理员-78', 'admin-78', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (79, '管理员-79', 'admin-79', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (80, '管理员-80', 'admin-80', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (81, '管理员-81', 'admin-81', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (82, '管理员-82', 'admin-82', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (83, '管理员-83', 'admin-83', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (84, '管理员-84', 'admin-84', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (85, '管理员-85', 'admin-85', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (86, '管理员-86', 'admin-86', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (87, '管理员-87', 'admin-87', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (88, '管理员-88', 'admin-88', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (89, '管理员-89', 'admin-89', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (90, '管理员-90', 'admin-90', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (91, '管理员-91', 'admin-91', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (92, '管理员-92', 'admin-92', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (93, '管理员-93', 'admin-93', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (94, '管理员-94', 'admin-94', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (95, '管理员-95', 'admin-95', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (96, '管理员-96', 'admin-96', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (97, '管理员-97', 'admin-97', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (98, '管理员-98', 'admin-98', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (99, '关联测试数据-1', 'relation-role-1', NULL, 0, NULL, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (100, '关联测试数据-2', 'relation-role-2', NULL, 0, NULL, 1, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (101, '关联测试数据-3', 'relation-role-3', NULL, 0, NULL, 2, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);

ALTER SEQUENCE "public"."sys_role_role_id_seq"
OWNED BY "public"."sys_role"."role_id";
SELECT setval('"public"."sys_role_role_id_seq"', 101, true);
ALTER TABLE "public"."sys_role" ADD CONSTRAINT "sys_role_pkey" PRIMARY KEY ("role_id");

DROP TABLE IF EXISTS "public"."sys_role_dept";
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

INSERT INTO "public"."sys_role_dept" VALUES (100, 2, 1, '2026-04-02 21:53:40.210895');
INSERT INTO "public"."sys_role_dept" VALUES (100, 4, 1, '2026-04-02 21:53:40.210895');
INSERT INTO "public"."sys_role_dept" VALUES (101, 3, 1, '2026-04-02 21:53:40.210895');
INSERT INTO "public"."sys_role_dept" VALUES (101, 4, 1, '2026-04-02 21:53:40.210895');

ALTER TABLE "public"."sys_role_dept" ADD CONSTRAINT "sys_role_dept_pkey" PRIMARY KEY ("role_id", "dept_id");
