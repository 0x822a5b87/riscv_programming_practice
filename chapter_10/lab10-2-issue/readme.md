# readme.md

以下代码会导致恒等映射异常，请问是为什么？

```c
static void create_identical_mapping(void)
{
	unsigned long start;
	unsigned long end;

	/*map memory*/
	start = (unsigned long)_text_boot; // 0x80200000
	end = DDR_END;					   // 0x81200000
	// (pgd_t *) idmap_pg_dir =			  0x80208000
	__create_pgd_mapping((pgd_t *)idmap_pg_dir, start, start,
			end - start, PAGE_KERNEL,
			early_pgtable_alloc,
			0);
	printk("map memory done\n");
}
```

这里，在做恒等映射的过程中，`.text` 区代码缺少可执行权限。