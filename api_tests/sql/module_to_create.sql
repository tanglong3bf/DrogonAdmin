DROP TABLE IF EXISTS "public"."sys_module";

DROP SEQUENCE IF EXISTS "public"."sys_module_module_id_seq";
CREATE SEQUENCE "public"."sys_module_module_id_seq" 
INCREMENT 1
MINVALUE  1
MAXVALUE 2147483647
START 1
CACHE 1;

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

-- 模块表数据
INSERT INTO public.sys_module VALUES (1, '占位模块-1', NULL, 0, NULL, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);
INSERT INTO public.sys_module VALUES (2, '占位模块-2', NULL, 1, NULL, 1, '2026-07-05 14:46:05.000000', 1, '2026-07-05 14:46:05.000000', NULL, NULL);

ALTER SEQUENCE "public"."sys_module_module_id_seq"
OWNED BY "public"."sys_module"."module_id";
SELECT setval('"public"."sys_module_module_id_seq"', 2, true);

ALTER TABLE "public"."sys_module" ADD CONSTRAINT "sys_module_pkey" PRIMARY KEY ("module_id");
