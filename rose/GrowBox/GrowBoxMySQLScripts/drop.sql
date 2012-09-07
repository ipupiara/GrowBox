USE growdb;
REVOKE INSERT, DELETE, SELECT ON growdata FROM 'grower'@'%';
drop user 'grower'@'%';
drop database growdb;
show databases;