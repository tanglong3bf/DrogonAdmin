-- 用户更新测试数据准备

DROP TABLE IF EXISTS "public"."sys_user_role";
DROP TABLE IF EXISTS "public"."sys_role_dept";
DROP TABLE IF EXISTS "public"."sys_user";
DROP TABLE IF EXISTS "public"."sys_role";
DROP TABLE IF EXISTS "public"."sys_dept";
DROP SEQUENCE IF EXISTS "public"."sys_user_user_id_seq";
DROP SEQUENCE IF EXISTS "public"."sys_role_role_id_seq";
DROP SEQUENCE IF EXISTS "public"."sys_dept_dept_id_seq";

-- 创建部门表
CREATE SEQUENCE "public"."sys_dept_dept_id_seq"
INCREMENT 1 MINVALUE 1 MAXVALUE 2147483647 START 1 CACHE 1;
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
);

-- 创建角色表
CREATE SEQUENCE "public"."sys_role_role_id_seq"
INCREMENT 1 MINVALUE 1 MAXVALUE 2147483647 START 1 CACHE 1;
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
);

-- 创建用户表
CREATE SEQUENCE "public"."sys_user_user_id_seq"
INCREMENT 1 MINVALUE 1 MAXVALUE 2147483647 START 1 CACHE 1;
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
);

-- 创建角色部门关联表
CREATE TABLE "public"."sys_role_dept" (
  "role_id" int4 NOT NULL,
  "dept_id" int4 NOT NULL,
  "created_by" int4 NOT NULL,
  "created_time" timestamp(6) NOT NULL
);

-- 创建用户角色关联表
CREATE TABLE "public"."sys_user_role" (
  "user_id" int4 NOT NULL,
  "role_id" int4 NOT NULL,
  "created_by" int4 NOT NULL,
  "created_time" timestamp(6) NOT NULL
);

-- 部门数据
INSERT INTO "public"."sys_dept" VALUES (1, '技术部', 0, NULL, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
-- 软删除
INSERT INTO "public"."sys_dept" VALUES (2, '财务部', 1, NULL, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_dept" VALUES (3, '人事部', 2, NULL, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);

-- 角色部门关联数据
-- 白名单
INSERT INTO "public"."sys_role_dept" VALUES (1, 1, 1, '2026-04-01 00:00:00');
-- 黑名单
INSERT INTO "public"."sys_role_dept" VALUES (2, 3, 1, '2026-04-01 00:00:00');

-- 角色数据
INSERT INTO "public"."sys_role" VALUES (1, '角色-1', 'role-1', '白名单角色', 0, NULL, 1, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (2, '角色-2', 'role-2', '黑名单角色', 0, NULL, 2, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (3, '角色-3', 'role-3', '每个部门限制用户数量为1', 2, 1, 2, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);

-- 用户角色关联数据
INSERT INTO "public"."sys_user_role" VALUES (8, 1, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user_role" VALUES (9, 2, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user_role" VALUES (10, 3, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user_role" VALUES (11, 3, 1, '2026-04-01 00:00:00');

-- 用户1: 已删除
-- 用户2: 软删除的用户
INSERT INTO "public"."sys_user" VALUES (2, 'normal_user', '123456', '普通用户', '#', 0, 1, '18800000002', 'normal@test.com', 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00');
-- 用户3: 用户用于测试无更新 (所有字段和旧值相同)
INSERT INTO "public"."sys_user" VALUES (3, 'no_update_user', '123456', '无更新测试用户', '#', 1, 1, '18800000003', 'noupdate@test.com', 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
-- 用户4: 用户用于测试更新其他字段 (除部门id和角色id之外)
INSERT INTO "public"."sys_user" VALUES (4, 'update_fields_user', '123456', '更新字段测试用户', '#', 1, 1, '18800000004', 'updatefields@test.com', 1, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
-- 用户5: 用户用于测试清空手机和邮箱
INSERT INTO "public"."sys_user" VALUES (5, 'clear_fields_user', '123456', '清空字段测试用户', '#', 1, 1, '18800000005', 'updaterole@test.com', 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
-- 转移至不存在的部门
INSERT INTO "public"."sys_user" VALUES (6, 'update_dept_user', '123456', '转移部门失败测试用户', '#', 1, 1, '18800000006', 'updatedept@test.com', 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
-- 正常转移部门
INSERT INTO "public"."sys_user" VALUES (7, 'user-7', '123456', '用户-7', '#', 1, 1, '18800000007', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
-- 角色白名单到其他
INSERT INTO "public"."sys_user" VALUES (8, 'user-8', '123456', '用户-8', '#', 1, 1, '18800000008', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
-- 角色普通到黑名单
INSERT INTO "public"."sys_user" VALUES (9, 'user-9', '123456', '用户-9', '#', 1, 1, '18800000009', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
-- 占位用户
INSERT INTO "public"."sys_user" VALUES (10, 'user-10', '123456', '用户-10', '#', 1, 3, '18800000010', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
-- 转移到已被使用角色的部门
INSERT INTO "public"."sys_user" VALUES (11, 'user-11', '123456', '用户-11', '#', 1, 1, '18800000011', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);


-- 设置序列起始值
ALTER SEQUENCE "public"."sys_dept_dept_id_seq" OWNED BY "public"."sys_dept"."dept_id";
SELECT setval('"public"."sys_dept_dept_id_seq"', 3, true);
ALTER SEQUENCE "public"."sys_role_role_id_seq" OWNED BY "public"."sys_role"."role_id";
SELECT setval('"public"."sys_role_role_id_seq"', 3, true);
ALTER SEQUENCE "public"."sys_user_user_id_seq" OWNED BY "public"."sys_user"."user_id";
SELECT setval('"public"."sys_user_user_id_seq"', 11, true);

-- 添加主键约束
ALTER TABLE "public"."sys_dept" ADD CONSTRAINT "sys_dept_pkey" PRIMARY KEY ("dept_id");
ALTER TABLE "public"."sys_role" ADD CONSTRAINT "sys_role_pkey" PRIMARY KEY ("role_id");
ALTER TABLE "public"."sys_user" ADD CONSTRAINT "sys_user_pkey" PRIMARY KEY ("user_id");
ALTER TABLE "public"."sys_role_dept" ADD CONSTRAINT "sys_role_dept_pkey" PRIMARY KEY ("role_id", "dept_id");
ALTER TABLE "public"."sys_user_role" ADD CONSTRAINT "sys_user_role_pkey" PRIMARY KEY ("user_id", "role_id");
