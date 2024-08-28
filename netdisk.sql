CREATE TABLE `friend` ( 
  `id` INT AUTO_INCREMENT NOT NULL,
  `uid_1` INT NOT NULL,
  `uid_2` INT NOT NULL,
  `status` INT NOT NULL COMMENT '
        0->朋友关系，
        1->等待验证，
        2->已拒绝,
    ' ,
  `lastUpdateTime` DATETIME NOT NULL,
  CONSTRAINT `PRIMARY` PRIMARY KEY (`id`),
  CONSTRAINT `uid_1` UNIQUE (`uid_1`, `uid_2`)
);
CREATE TABLE `user` ( 
  `id` INT AUTO_INCREMENT NOT NULL,
  `email` VARCHAR(64) NOT NULL,
  `passwd` VARCHAR(32) NOT NULL,
  `userName` VARCHAR(32) NOT NULL,
  `status` INT NOT NULL COMMENT '账号状态' ,
  `isvip` TINYINT NOT NULL DEFAULT 0  COMMENT '是否为vip' ,
  CONSTRAINT `PRIMARY` PRIMARY KEY (`id`),
  CONSTRAINT `email` UNIQUE (`email`)
);
ALTER TABLE `friend` ADD CONSTRAINT `friend_ibfk_1` FOREIGN KEY (`uid_1`) REFERENCES `user` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION;
ALTER TABLE `friend` ADD CONSTRAINT `friend_ibfk_2` FOREIGN KEY (`uid_2`) REFERENCES `user` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION;

