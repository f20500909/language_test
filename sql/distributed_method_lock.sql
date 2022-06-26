
CREATE TABLE  `method_lock`
(
    `id` int(11) NOT NULL AUTO_INCREMENT COMMENT `主键`,
    `method_name` varchar(64) NOT NULL DEFAULT `` COMMENT `锁定的方法名称`,

)