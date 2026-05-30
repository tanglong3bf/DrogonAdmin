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

INSERT INTO "public"."sys_dept" VALUES (1, '部门-1', 0, NULL, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (2, '部门-2', 0, NULL, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (3, '部门-3', 0, NULL, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (4, '部门-4', 0, NULL, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (5, '部门-5', 0, NULL, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', 1, '2026-01-10  21:47:30');

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
INSERT INTO "public"."sys_role" VALUES (10, '管理员-10', 'admin-10', NULL, 1, 3, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (11, '管理员-11', 'admin-11', NULL, 1, 3, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (12, '管理员-12', 'admin-12', NULL, 1, 3, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (13, '管理员-13', 'admin-13', NULL, 1, 3, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (14, '管理员-14', 'admin-14', NULL, 2, 3, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (15, '管理员-15', 'admin-15', NULL, 2, 3, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (16, '管理员-16', 'admin-16', NULL, 2, 3, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (17, '管理员-17', 'admin-17', NULL, 2, 3, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (18, '管理员-18', 'admin-18', NULL, 2, 3, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (19, '管理员-19', 'admin-19', NULL, 1, 3, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (20, '管理员-20', 'admin-20', NULL, 1, 3, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (21, '管理员-21', 'admin-21', NULL, 1, 3, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (22, '管理员-22', 'admin-22', NULL, 1, 3, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (23, '管理员-23', 'admin-23', NULL, 1, 3, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (24, '管理员-24', 'admin-24', NULL, 1, 3, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (25, '管理员-25', 'admin-25', NULL, 2, 3, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (26, '管理员-26', 'admin-26', NULL, 2, 3, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (27, '管理员-27', 'admin-27', NULL, 2, 3, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (28, '管理员-28', 'admin-28', NULL, 2, 3, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (29, '管理员-29', 'admin-29', NULL, 2, 3, 0, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
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
INSERT INTO "public"."sys_role" VALUES (40, '管理员-40', 'admin-40', NULL, 0, NULL, 1, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (41, '管理员-41', 'admin-41', NULL, 0, NULL, 1, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (42, '管理员-42', 'admin-42', NULL, 0, NULL, 1, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (43, '管理员-43', 'admin-43', NULL, 0, NULL, 1, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (44, '管理员-44', 'admin-44', NULL, 0, NULL, 2, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (45, '管理员-45', 'admin-45', NULL, 0, NULL, 2, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (46, '管理员-46', 'admin-46', NULL, 0, NULL, 2, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (47, '管理员-47', 'admin-47', NULL, 0, NULL, 2, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);

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

ALTER TABLE "public"."sys_role_dept" ADD CONSTRAINT "sys_role_dept_pkey" PRIMARY KEY ("role_id", "dept_id");

INSERT INTO "public"."sys_role_dept" VALUES (40, 1, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_role_dept" VALUES (40, 2, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_role_dept" VALUES (41, 1, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_role_dept" VALUES (41, 2, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_role_dept" VALUES (42, 1, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_role_dept" VALUES (42, 2, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_role_dept" VALUES (43, 1, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_role_dept" VALUES (43, 2, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_role_dept" VALUES (44, 1, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_role_dept" VALUES (44, 2, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_role_dept" VALUES (45, 1, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_role_dept" VALUES (45, 2, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_role_dept" VALUES (46, 1, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_role_dept" VALUES (46, 2, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_role_dept" VALUES (47, 1, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_role_dept" VALUES (47, 2, 1, '2026-04-02 21:53:40');

DROP TABLE IF EXISTS "public"."sys_user_role";
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

INSERT INTO "public"."sys_user_role" VALUES (1, 3, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (1, 4, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (2, 4, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (3, 4, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (1, 5, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (2, 5, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (3, 5, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (4, 5, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (1, 7, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (2, 7, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (3, 7, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (4, 7, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (1, 8, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (2, 8, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (5, 8, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (6, 8, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (7, 8, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (8, 8, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (1, 9, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (2, 9, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (5, 9, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (6, 9, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (7, 9, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (8, 9, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (9, 9, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (1, 12, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (2, 12, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (1, 16, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (2, 16, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (1, 17, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (2, 17, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (7, 17, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (1, 20, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (2, 20, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (7, 20, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (1, 22, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (2, 22, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (5, 22, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (6, 22, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (7, 22, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (8, 22, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (9, 22, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (1, 24, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (5, 24, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (6, 24, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (7, 24, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (8, 24, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (10, 24, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (11, 24, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (1, 27, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (2, 27, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (5, 27, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (7, 27, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (1, 29, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (2, 29, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (3, 29, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (4, 29, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (5, 29, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (7, 29, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (3, 34, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (1, 39, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (1, 41, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (1, 42, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (2, 43, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (3, 45, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (4, 46, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_user_role" VALUES (3, 47, 1, '2026-04-02 21:53:40');

ALTER TABLE "public"."sys_user_role" ADD CONSTRAINT "sys_user_role_pkey" PRIMARY KEY ("user_id", "role_id");

DROP TABLE IF EXISTS "public"."sys_user";
DROP SEQUENCE IF EXISTS "public"."sys_user_user_id_seq";
CREATE SEQUENCE "public"."sys_user_user_id_seq" 
INCREMENT 1
MINVALUE  1
MAXVALUE 2147483647
START 1
CACHE 1;
CREATE TABLE "public"."sys_user" (
  "user_id" int4 NOT NULL DEFAULT nextval('sys_user_user_id_seq'::regclass),
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

INSERT INTO "public"."sys_user" VALUES (1, 'user-1', '123456', '用户-1', '#', 1, 1, NULL, NULL, 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (2, 'user-2', '123456', '用户-2', '#', 1, 2, NULL, NULL, 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (3, 'user-3', '123456', '用户-3', '#', 1, 3, NULL, NULL, 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (4, 'user-4', '123456', '用户-4', '#', 1, 4, NULL, NULL, 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (5, 'user-5', '123456', '用户-5', '#', 1, 1, NULL, NULL, 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (6, 'user-6', '123456', '用户-6', '#', 1, 1, NULL, NULL, 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (7, 'user-7', '123456', '用户-7', '#', 1, 2, NULL, NULL, 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (8, 'user-8', '123456', '用户-8', '#', 1, 2, NULL, NULL, 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (9, 'user-9', '123456', '用户-9', '#', 1, 2, NULL, NULL, 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (10, 'user-10', '123456', '用户-10', '#', 1, 1, NULL, NULL, 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (11, 'user-11', '123456', '用户-11', '#', 1, 1, NULL, NULL, 0, 1, '2026-04-03 22:28:44', 1, '2026-04-03 22:28:44', NULL, NULL);

ALTER SEQUENCE "public"."sys_user_user_id_seq"
OWNED BY "public"."sys_user"."user_id";
SELECT setval('"public"."sys_user_user_id_seq"', 3, true);
ALTER TABLE "public"."sys_user" ADD CONSTRAINT "sys_user_pkey" PRIMARY KEY ("user_id");
