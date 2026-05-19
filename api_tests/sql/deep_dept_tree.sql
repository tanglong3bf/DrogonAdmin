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

INSERT INTO "public"."sys_dept" VALUES (1, '钱途无量有限公司', 0, NULL, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (2, '钱途无量有限公司', 0, 1, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (3, '钱途无量有限公司', 0, 2, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (4, '钱途无量有限公司', 0, 3, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (5, '钱途无量有限公司', 0, 4, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (6, '钱途无量有限公司', 0, 5, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (7, '钱途无量有限公司', 0, 6, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (8, '钱途无量有限公司', 0, 7, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (9, '钱途无量有限公司', 0, 8, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (10, '钱途无量有限公司', 0, 9, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (11, '钱途无量有限公司', 0, 10, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (12, '钱途无量有限公司', 0, 11, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (13, '钱途无量有限公司', 0, 12, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (14, '钱途无量有限公司', 0, 13, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (15, '钱途无量有限公司', 0, 14, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (16, '钱途无量有限公司', 0, 15, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (17, '钱途无量有限公司', 0, 16, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (18, '钱途无量有限公司', 0, 17, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (19, '钱途无量有限公司', 0, 18, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (20, '钱途无量有限公司', 0, 19, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (21, '钱途无量有限公司', 0, 20, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (22, '钱途无量有限公司', 0, 21, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (23, '钱途无量有限公司', 0, 22, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (24, '钱途无量有限公司', 0, 23, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (25, '钱途无量有限公司', 0, 24, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (26, '钱途无量有限公司', 0, 25, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (27, '钱途无量有限公司', 0, 26, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (28, '钱途无量有限公司', 0, 27, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (29, '钱途无量有限公司', 0, 28, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (30, '钱途无量有限公司', 0, 29, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (31, '钱途无量有限公司', 0, 30, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (32, '钱途无量有限公司', 0, 31, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (33, '钱途无量有限公司', 0, 32, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (34, '钱途无量有限公司', 0, 33, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (35, '钱途无量有限公司', 0, 34, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (36, '钱途无量有限公司', 0, 35, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (37, '钱途无量有限公司', 0, 36, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (38, '钱途无量有限公司', 0, 37, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (39, '钱途无量有限公司', 0, 38, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (40, '钱途无量有限公司', 0, 39, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (41, '钱途无量有限公司', 0, 40, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (42, '钱途无量有限公司', 0, 41, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (43, '钱途无量有限公司', 0, 42, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (44, '钱途无量有限公司', 0, 43, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (45, '钱途无量有限公司', 0, 44, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (46, '钱途无量有限公司', 0, 45, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (47, '钱途无量有限公司', 0, 46, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (48, '钱途无量有限公司', 0, 47, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (49, '钱途无量有限公司', 0, 48, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (50, '钱途无量有限公司', 0, 49, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (51, '钱途无量有限公司', 0, 50, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (52, '钱途无量有限公司', 0, 51, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (53, '钱途无量有限公司', 0, 52, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (54, '钱途无量有限公司', 0, 53, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (55, '钱途无量有限公司', 0, 54, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (56, '钱途无量有限公司', 0, 55, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (57, '钱途无量有限公司', 0, 56, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (58, '钱途无量有限公司', 0, 57, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (59, '钱途无量有限公司', 0, 58, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (60, '钱途无量有限公司', 0, 59, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (61, '钱途无量有限公司', 0, 60, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (62, '钱途无量有限公司', 0, 61, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (63, '钱途无量有限公司', 0, 62, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (64, '钱途无量有限公司', 0, 63, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (65, '钱途无量有限公司', 0, 64, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (66, '钱途无量有限公司', 0, 65, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (67, '钱途无量有限公司', 0, 66, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (68, '钱途无量有限公司', 0, 67, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (69, '钱途无量有限公司', 0, 68, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (70, '钱途无量有限公司', 0, 69, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (71, '钱途无量有限公司', 0, 70, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (72, '钱途无量有限公司', 0, 71, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (73, '钱途无量有限公司', 0, 72, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (74, '钱途无量有限公司', 0, 73, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (75, '钱途无量有限公司', 0, 74, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (76, '钱途无量有限公司', 0, 75, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (77, '钱途无量有限公司', 0, 76, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (78, '钱途无量有限公司', 0, 77, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (79, '钱途无量有限公司', 0, 78, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (80, '钱途无量有限公司', 0, 79, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (81, '钱途无量有限公司', 0, 80, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (82, '钱途无量有限公司', 0, 81, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (83, '钱途无量有限公司', 0, 82, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (84, '钱途无量有限公司', 0, 83, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (85, '钱途无量有限公司', 0, 84, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (86, '钱途无量有限公司', 0, 85, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (87, '钱途无量有限公司', 0, 86, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (88, '钱途无量有限公司', 0, 87, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (89, '钱途无量有限公司', 0, 88, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (90, '钱途无量有限公司', 0, 89, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (91, '钱途无量有限公司', 0, 90, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (92, '钱途无量有限公司', 0, 91, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (93, '钱途无量有限公司', 0, 92, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (94, '钱途无量有限公司', 0, 93, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (95, '钱途无量有限公司', 0, 94, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (96, '钱途无量有限公司', 0, 95, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (97, '钱途无量有限公司', 0, 96, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (98, '钱途无量有限公司', 0, 97, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (99, '钱途无量有限公司', 0, 98, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);
INSERT INTO "public"."sys_dept" VALUES (100, '钱途无量有限公司', 0, 99, 1, '2026-01-10 21:47:30', 1, '2026-01-10 21:47:30', NULL, NULL);

ALTER SEQUENCE "public"."sys_dept_dept_id_seq"
OWNED BY "public"."sys_dept"."dept_id";
SELECT setval('"public"."sys_dept_dept_id_seq"', 100, true);
ALTER TABLE "public"."sys_dept" ADD CONSTRAINT "sys_dept_pkey" PRIMARY KEY ("dept_id");
