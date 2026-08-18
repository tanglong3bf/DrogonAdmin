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
  "version" int4 NOT NULL DEFAULT 0,
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
  "version" int4 NOT NULL DEFAULT 0,
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
  "password" char(60) COLLATE "pg_catalog"."default" NOT NULL,
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
INSERT INTO "public"."sys_dept" VALUES (1, '技术部', 0, NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
-- 软删除
INSERT INTO "public"."sys_dept" VALUES (2, '财务部', 1, NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_dept" VALUES (3, '人事部', 2, NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (4, '市场部', 2, NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);

-- 角色部门关联数据
-- 白名单
INSERT INTO "public"."sys_role_dept" VALUES (1, 1, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_role_dept" VALUES (7, 3, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_role_dept" VALUES (8, 4, 1, '2026-04-01 00:00:00');
-- 黑名单
INSERT INTO "public"."sys_role_dept" VALUES (2, 3, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_role_dept" VALUES (9, 1, 1, '2026-04-01 00:00:00');

-- 角色数据
INSERT INTO "public"."sys_role" VALUES (1, '角色-1', 'role-1', '技术部白名单', 0, NULL, 1, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (2, '角色-2', 'role-2', '人事部黑名单', 0, NULL, 2, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (3, '角色-3', 'role-3', '每个部门限制用户数量为1', 2, 1, 0, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (4, '角色-4', 'role-4', '软删除的角色', 0, NULL, 0, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_role" VALUES (5, '角色-5', 'role-5', '正常的角色', 0, NULL, 0, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (6, '角色-6', 'role-6', '总数量限制为1', 1, 1, 0, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (7, '角色-7', 'role-7', '人事部白名单', 0, NULL, 1, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (8, '角色-8', 'role-8', '市场部白名单', 2, 1, 1, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (9, '角色-9', 'role-9', '技术部黑名单', 0, NULL, 2, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_role" VALUES (10, '角色-10', 'role-10', '正常角色', 0, NULL, 0, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);

-- 用户角色关联数据
INSERT INTO "public"."sys_user_role" VALUES (8, 1, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user_role" VALUES (9, 2, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user_role" VALUES (10, 3, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user_role" VALUES (11, 3, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user_role" VALUES (14, 1, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user_role" VALUES (14, 2, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user_role" VALUES (14, 5, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user_role" VALUES (18, 6, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user_role" VALUES (20, 1, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user_role" VALUES (21, 8, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user_role" VALUES (22, 1, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user_role" VALUES (23, 1, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user_role" VALUES (24, 7, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user_role" VALUES (25, 5, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user_role" VALUES (26, 5, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user_role" VALUES (27, 5, 1, '2026-04-01 00:00:00');
INSERT INTO "public"."sys_user_role" VALUES (28, 5, 1, '2026-04-01 00:00:00');

-- 用户1: 已删除
-- 用户2: 软删除的用户
INSERT INTO "public"."sys_user" VALUES (2, 'normal_user', '$2a$10$NeYlQ2HsBvWcrsTsDiGmJuXYK32wVqQytfYzpQrVJqcKxyJkIcxmq', '普通用户', '#', 0, 1, '18800000002', 'normal@test.com', 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00');
-- 用户3: 用户用于测试无更新 (所有字段和旧值相同)
INSERT INTO "public"."sys_user" VALUES (3, 'no_update_user', '$2a$10$KPXW8nCM7sqDXmwKd9aJkOieXsX9WmdyjdbR9rdsfVZzcIpXVDZn6', '无更新测试用户', '#', 1, 1, '18800000003', 'noupdate@test.com', 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
-- 用户4: 用户用于测试更新其他字段 (除部门id和角色id之外)
INSERT INTO "public"."sys_user" VALUES (4, 'update_fields_user', '$2a$10$NueYJq7iqSod1ac2BP/VjOB/CiLMHrNc7uYammE.RVBFuARYknn7G', '更新字段测试用户', '#', 1, 1, '18800000004', 'updatefields@test.com', 1, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
-- 用户5: 用户用于测试清空手机和邮箱
INSERT INTO "public"."sys_user" VALUES (5, 'clear_fields_user', '$2a$10$.OX7JM1pdiooUJoE1yr/nuK0uxBOnZY4AmtjvS7Qn5/6YhMYD6pl2', '清空字段测试用户', '#', 1, 1, '18800000005', 'updaterole@test.com', 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
-- 转移至不存在的部门
INSERT INTO "public"."sys_user" VALUES (6, 'update_dept_user', '$2a$10$jwwLkBi.oCnTxkIdETHn5.0WAJ.xT0aIrQR3CdOFAZuQbpjG/V5SK', '转移部门失败测试用户', '#', 1, 1, '18800000006', 'updatedept@test.com', 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
-- 正常转移部门
INSERT INTO "public"."sys_user" VALUES (7, 'user-7', '$2a$10$Df2zrx5vjY5BTC9Q.M6Gw.QStinnhWnwRG50vmT7nouqVny7k0c5G', '用户-7', '#', 1, 1, '18800000007', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
-- 角色白名单到其他
INSERT INTO "public"."sys_user" VALUES (8, 'user-8', '$2a$10$SUgFH/gP75PkZAQM0/70AeyFVMv/JHCBPz6Oky3ehVM.ijg2RCQae', '用户-8', '#', 1, 1, '18800000008', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
-- 角色普通到黑名单
INSERT INTO "public"."sys_user" VALUES (9, 'user-9', '$2a$10$cqNJaKViYUZ48Go2QDO5du0FaauRhpIkup5sHnaoQywHvjeVyDl3y', '用户-9', '#', 1, 1, '18800000009', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
-- 占位用户
INSERT INTO "public"."sys_user" VALUES (10, 'user-10', '$2a$10$dcE2WNrBtGzMAcKQXGXNv.hn30xgNon/tNyXe4OABFht744/M2AKS', '用户-10', '#', 1, 3, '18800000010', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
-- 转移到已被使用角色的部门
INSERT INTO "public"."sys_user" VALUES (11, 'user-11', '$2a$10$On2WBmS0NjXcc2kxCtMSTuHbu0rwnozswmDdffIsE3JF.YTFDC0gq', '用户-11', '#', 1, 1, '18800000011', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
-- 为其赋予不存在的角色
INSERT INTO "public"."sys_user" VALUES (12, 'user-12', '$2a$10$2AfRi5F0A7.NHbPB40S0aebhnbUbmy8CcfjOhoUNxSFW.liphuVf6', '用户-12', '#', 1, 1, '18800000012', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
-- 正常赋予角色
INSERT INTO "public"."sys_user" VALUES (13, 'user-13', '$2a$10$olldKlBFKKvgqQdzEHCy/e6o90P5jy538YjixmkSwyt10IEYbyp02', '用户-13', '#', 1, 1, '18800000013', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
-- 已拥有三个角色，将被清空
INSERT INTO "public"."sys_user" VALUES (14, 'user-14', '$2a$10$q5jAgM.tAtVtUx1dQf1JU.bYte0e57ATucgoNXxGJujiJBWazAoc2', '用户-14', '#', 1, 1, '18800000014', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
-- 分配有白名单的角色，但用户不在白名单部门里
INSERT INTO "public"."sys_user" VALUES (15, 'user-15', '$2a$10$KPA1.FPuLuODKsH.FA4FseRLOwvR8Fxg7sEww/jbkFGXDlcVaFaEK', '用户-15', '#', 1, 3, '18800000015', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
-- 分配有黑名单的角色，且用户在黑名单部门里
INSERT INTO "public"."sys_user" VALUES (16, 'user-16', '$2a$10$tVJ/a8QfCQzgUU7ytdw40OuQ0its2k0zoNd6XqIixF3nBJ3TPv7ni', '用户-16', '#', 1, 3, '18800000016', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
-- 待分配角色，分配后超出每部门数量限制
INSERT INTO "public"."sys_user" VALUES (17, 'user-17', '$2a$10$0FKfAL/gAQxnjGGH7Yk4B.N7lTQ5eDeJmus2XtaqyPxtkrxwUKTji', '用户-17', '#', 1, 3, '18800000017', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
-- 占位用户
INSERT INTO "public"."sys_user" VALUES (18, 'user-18', '$2a$10$v/fzvfd1o53NjShWQgh1QeJRjBSVOsregJJbGTgjSrjb.6RPMA/aa', '用户-18', '#', 1, 1, '18800000018', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
-- 待分配角色，分配后超出总数量限制
INSERT INTO "public"."sys_user" VALUES (19, 'user-19', '$2a$10$DZH32Q7k70kPM10erb7xseHGICSgf3BsNYdd7a2tSWgMs/o9FBypy', '用户-19', '#', 1, 3, '18800000019', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (20, 'user-20', '$2a$10$1mhy7pfBztWsBbAYmZinA.yZUcyclWrI0YYTx02yhf/E94yBQLoWS', '用户-20', '#', 1, 1, '18800000020', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (21, 'user-21', '$2a$10$8BDK4qQ.doL/V.G6jn16Z.Vawd3GfJKymNXJSrKC29qUh9fkfA9KS', '用户-21', '#', 1, 4, '18800000020', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (22, 'user-22', '$2a$10$A/9tkKaIoryts8HdEy79TOExxoRVwej4iDHm7WFwCuUvCPEAjT1n6', '用户-22', '#', 1, 1, '18800000020', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (23, 'user-23', '$2a$10$nUWTRnnrCh2eV7Dmn9FwRuOzWy5TKHddZvZB7mxKHta2P6U/tX2NK', '用户-23', '#', 1, 1, '18800000020', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (24, 'user-24', '$2a$10$xqEJvV8WIcuIFYyzyxrkrOSVpZpFU8PlU.r6f0fvhYa1hBGYl4gwW', '用户-24', '#', 1, 3, '18800000020', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (25, 'user-25', '$2a$10$kDYkrx14vc0L0QlnPV8sVOCh4Ci1h6UHJNI9IIDWCeWVqvLUGQNCm', '用户-25', '#', 1, 1, '18800000020', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (26, 'user-26', '$2a$10$Xk4wG/RMl779lcEyEb9vTun2pdrrg9fm8u2xWWc0dHN2An1fPCMaO', '用户-26', '#', 1, 1, '18800000020', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (27, 'user-27', '$2a$10$a/L82DicaCkCucLsKbMDPOK2z0.EiO6KGFYhX.EgyXJvWi5fVn9PO', '用户-27', '#', 1, 1, '18800000020', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);
INSERT INTO "public"."sys_user" VALUES (28, 'user-28', '$2a$10$nSGnBWigHB3h.nUwwTlLXOXOUuTPRG5M2RuiZQFtMKx6FK6tF1c7K', '用户-28', '#', 1, 3, '18800000020', NULL, 0, 1, '2026-04-01 00:00:00', 1, '2026-04-01 00:00:00', NULL, NULL);


-- 设置序列起始值
ALTER SEQUENCE "public"."sys_dept_dept_id_seq" OWNED BY "public"."sys_dept"."dept_id";
SELECT setval('"public"."sys_dept_dept_id_seq"', 4, true);
ALTER SEQUENCE "public"."sys_role_role_id_seq" OWNED BY "public"."sys_role"."role_id";
SELECT setval('"public"."sys_role_role_id_seq"', 10, true);
ALTER SEQUENCE "public"."sys_user_user_id_seq" OWNED BY "public"."sys_user"."user_id";
SELECT setval('"public"."sys_user_user_id_seq"', 28, true);

-- 添加主键约束
ALTER TABLE "public"."sys_dept" ADD CONSTRAINT "sys_dept_pkey" PRIMARY KEY ("dept_id");
ALTER TABLE "public"."sys_role" ADD CONSTRAINT "sys_role_pkey" PRIMARY KEY ("role_id");
ALTER TABLE "public"."sys_user" ADD CONSTRAINT "sys_user_pkey" PRIMARY KEY ("user_id");
ALTER TABLE "public"."sys_role_dept" ADD CONSTRAINT "sys_role_dept_pkey" PRIMARY KEY ("role_id", "dept_id");
ALTER TABLE "public"."sys_user_role" ADD CONSTRAINT "sys_user_role_pkey" PRIMARY KEY ("user_id", "role_id");
