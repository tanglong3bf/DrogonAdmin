DROP TABLE IF EXISTS "public"."sys_module";
DROP TABLE IF EXISTS "public"."sys_action";
DROP TABLE IF EXISTS "public"."sys_action_priority";

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

CREATE TABLE "public"."sys_module" (
  "module_id" int4 NOT NULL DEFAULT nextval('sys_module_module_id_seq'::regclass),
  "name" varchar(50) COLLATE "pg_catalog"."default" NOT NULL,
  "description" varchar(255) COLLATE "pg_catalog"."default",
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
COMMENT ON COLUMN "public"."sys_module"."module_id" IS '模块id';
COMMENT ON COLUMN "public"."sys_module"."name" IS '模块名称';
COMMENT ON COLUMN "public"."sys_module"."description" IS '模块描述';
COMMENT ON COLUMN "public"."sys_module"."sort_num" IS '模块排序';
COMMENT ON COLUMN "public"."sys_module"."parent_id" IS '父模块id';
COMMENT ON COLUMN "public"."sys_module"."version" IS '乐观锁版本号';
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
  "has_data_permission" bool NOT NULL DEFAULT false,
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
COMMENT ON COLUMN "public"."sys_action"."has_data_permission" IS '是否有数据权限';
COMMENT ON COLUMN "public"."sys_action"."module_id" IS '所属模块id';
COMMENT ON COLUMN "public"."sys_action"."created_by" IS '创建者';
COMMENT ON COLUMN "public"."sys_action"."created_time" IS '创建时间';
COMMENT ON COLUMN "public"."sys_action"."updated_by" IS '最新一次更新者';
COMMENT ON COLUMN "public"."sys_action"."updated_time" IS '最新一次更新时间';
COMMENT ON COLUMN "public"."sys_action"."deleted_by" IS '删除者';
COMMENT ON COLUMN "public"."sys_action"."deleted_time" IS '删除时间';

CREATE TABLE "public"."sys_action_priority" (
  "high_id" int4 NOT NULL,
  "low_id" int4 NOT NULL,
  "module_id" int4 NOT NULL,
  "created_by" int4 NOT NULL,
  "created_time" timestamp(6) NOT NULL
)
;
COMMENT ON COLUMN "public"."sys_action_priority"."high_id" IS '高优先级功能id';
COMMENT ON COLUMN "public"."sys_action_priority"."low_id" IS '低优先级功能id';
COMMENT ON COLUMN "public"."sys_action_priority"."module_id" IS '所属模块id';
COMMENT ON COLUMN "public"."sys_action_priority"."created_by" IS '创建者';
COMMENT ON COLUMN "public"."sys_action_priority"."created_time" IS '创建时间';

ALTER SEQUENCE "public"."sys_module_module_id_seq"
OWNED BY "public"."sys_module"."module_id";
SELECT setval('"public"."sys_module_module_id_seq"', 1, false);
ALTER SEQUENCE "public"."sys_action_action_id_seq"
OWNED BY "public"."sys_action"."action_id";
SELECT setval('"public"."sys_action_action_id_seq"', 1, false);

ALTER TABLE "public"."sys_module" ADD CONSTRAINT "sys_module_pkey" PRIMARY KEY ("module_id");
ALTER TABLE "public"."sys_action" ADD CONSTRAINT "sys_action_pkey" PRIMARY KEY ("action_id");
ALTER TABLE "public"."sys_action_priority" ADD CONSTRAINT "sys_action_priority_pkey" PRIMARY KEY ("high_id", "low_id");
