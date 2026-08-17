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

INSERT INTO "public"."sys_dept" VALUES (1, '部门-1', 0, NULL, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (2, '部门-2', 0, NULL, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (3, '部门-3', 0, NULL, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (4, '部门-4', 0, NULL, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (5, '部门-5', 0, NULL, 0, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30');

ALTER SEQUENCE "public"."sys_dept_dept_id_seq"
OWNED BY "public"."sys_dept"."dept_id";
SELECT setval('"public"."sys_dept_dept_id_seq"', 5, true);
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
INSERT INTO "public"."sys_role" VALUES (2, '管理员-2', 'admin-2', NULL, 0, NULL, 1, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (3, '管理员-3', 'admin-3', NULL, 0, NULL, 2, 1, '2026-03-29 00:00:00', 1, '2026-04-02 21:53:40', NULL, NULL);

ALTER SEQUENCE "public"."sys_role_role_id_seq"
OWNED BY "public"."sys_role"."role_id";
SELECT setval('"public"."sys_role_role_id_seq"', 3, true);
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

INSERT INTO "public"."sys_role_dept" VALUES (2, 2, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_role_dept" VALUES (2, 4, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_role_dept" VALUES (3, 3, 1, '2026-04-02 21:53:40');
INSERT INTO "public"."sys_role_dept" VALUES (3, 4, 1, '2026-04-02 21:53:40');
