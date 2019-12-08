select 
	'table' node_type,
	c.relname || coalesce(' <i><span class="light"> ' ||
		case 
			when c.relpersistence = 'u'::"char" then 'unlogged'
			when c.relkind = 'f'::"char" then
				'&larr; ' ||
					(select srvname
					from pg_foreign_table t
						join pg_foreign_server s on t.ftserver = s.oid
					where t.ftrelid = c.oid)
			else ''
		end ||
		case when c.relkind = 'p'::"char" then 'partitioned' else '' end ||
		'</span></i>', '') ui_name,
	c.oid id,
	c.relkind::text tag,
	quote_ident(c.relname) "name",
	true allow_multiselect,
	'table.png' icon,
	c.relname sort1,
	c.relkind::text || c.relname sort2
from pg_class c
where c.relnamespace = $schema.id$ and
	(c.relkind = any (array['r'::"char", 'f'::"char", 'p'::"char"])) and 
	not pg_is_other_temp_schema(c.relnamespace) and 
	(
		pg_has_role(c.relowner, 'usage'::text) or 
		has_table_privilege(c.oid, 'select, insert, update, delete, truncate, references, trigger'::text) or 
		has_any_column_privilege(c.oid, 'select, insert, update, references'::text)
	)
/* if version 110000 */
	and
	(
		(
			$tables.id$ is null  and
			coalesce(c.relispartition, false) = false
		)
		or
		(
			coalesce(c.relispartition, false) = true and
			exists (
				select 1 
				from pg_inherits p
				where p.inhrelid = c.oid and p.inhparent = $tables.id$
			)
		)
	)
/* endif version */
order by c.relname desc