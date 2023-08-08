-- MySQL dump 10.13  Distrib 5.7.41, for Linux (x86_64)
--
-- Host: localhost    Database: olinejudge
-- ------------------------------------------------------
-- Server version	5.7.41

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Current Database: `olinejudge`
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `olinejudge` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `olinejudge`;

--
-- Table structure for table `administrators`
--

DROP TABLE IF EXISTS `administrators`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `administrators` (
  `UserId` int(11) NOT NULL AUTO_INCREMENT COMMENT '用户ID',
  `user` varchar(32) NOT NULL COMMENT '用户名',
  `password` int(11) NOT NULL COMMENT '密码',
  PRIMARY KEY (`UserId`),
  UNIQUE KEY `user` (`user`),
  UNIQUE KEY `user_2` (`user`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `administrators`
--

LOCK TABLES `administrators` WRITE;
/*!40000 ALTER TABLE `administrators` DISABLE KEYS */;
INSERT INTO `administrators` VALUES (1,'root',123456);
/*!40000 ALTER TABLE `administrators` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `joblist`
--

DROP TABLE IF EXISTS `joblist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `joblist` (
  `number` int(11) NOT NULL AUTO_INCREMENT COMMENT '工作编号',
  `name` varchar(64) NOT NULL COMMENT '公司名',
  `position` varchar(64) NOT NULL COMMENT '岗位名称',
  `salary` varchar(64) NOT NULL COMMENT '薪资待遇',
  `address` varchar(128) NOT NULL COMMENT '办公地址',
  `describe` varchar(10240) NOT NULL COMMENT '要求',
  `phone` varchar(128) NOT NULL COMMENT '联系方式',
  PRIMARY KEY (`number`)
) ENGINE=InnoDB AUTO_INCREMENT=15 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `joblist`
--

LOCK TABLES `joblist` WRITE;
/*!40000 ALTER TABLE `joblist` DISABLE KEYS */;
INSERT INTO `joblist` VALUES (1,'字节跳动','后端开发工程师-基础架构\n','26-30K * 12薪','北京','岗位职责\n团队介绍：字节跳动基础架构团队主要负责公司云基础建设，支撑着字节跳动旗下多款APP产品，如抖音、今日头条、番茄小说、西瓜视频、飞书、剪映等，同时也负责支持火山引擎公有云业务。迄今为止，我们通过云技术管理着百万量级的服务器构成的超大数据中心；我们通过字节深度优化的Kubernetes管理超过千万容器实例支持10万+微服务；我们还通过丰富的存储产品矩阵，如NewSQL、NoSQL、云存储等治理EB级的数据资产；我们积极拥抱开源和创新的软硬件架构，致力于构建业界领先的云基础设施，为整个公司的业务和客户发展保驾护航。我们热切期待对技术有追求、对大型系统有深刻见解的同学加入基础架构团队一起构建基础设施系统。 \n1、负责字节跳动分布式计算、分布式存储、分布式数据库、虚拟化、网络、Linux内核、研发平台、IaaS等基础设施的构建和优化； \n2、负责基础设施的可用性和稳定性保障。\n\n岗位要求\n1、2023届获得本科及以上学历，计算机相关专业；\n2、热爱计算机科学和互联网技术，擅长至少一门编程语言，包括但不仅限于：Java、C、C++、PHP、 Python、Golang等； \n3、掌握扎实的计算机基础知识，深入理解数据结构、算法和操作系统知识； \n4、有优秀的逻辑分析能力，能够对业务逻辑进行合理的抽象和拆分；\n5、有强烈的求知欲，优秀的学习和沟通能力；\n6、有云计算、分布式存储、研发平台类项目经历优先。\n','666-6666-6666'),(2,'测试公司1','测试岗位','测试薪资','测试地址','测试描述','测试电话'),(3,'测试公司2','测试岗位','测试薪资','测试薪资','测试描述','测试电话'),(4,'测试公司3','测试岗位','测试薪资','测试薪资','测试描述','测试电话'),(5,'测试公司4','测试岗位','测试薪资','测试薪资','测试描述','测试电话'),(6,'测试公司5','测试岗位','测试薪资','测试薪资','测试描述','测试电话'),(7,'录入测试','无无无','20k-30k','上海','暂无','12345'),(9,'测试公司6','测试岗位','10k-20k','岳阳','电话联系','123456789'),(10,'海康威视','软件开发工程师','面议','杭州','岗位职责\n1、根据公司产品及业务开发需求，进行嵌入式应用软件设计，开发，实现软件功能，并编写相应的开发文档；\n2、对已完成的系统软件进行调试，验证，测试，优化，提升系统或软件的可靠性，稳定性；\n3、对疑难问题进行跟进和解决。\n\n1、物联网设备配套应用软件、海外行业平台软件及相关组件开发；\n2、理解物联网产品和应用场景解决方案，参与产品需求分析、设计、开发和维护工作；\n3、参与产品需求、设计、规范等相关技术文档等编写；\n4、参与相关技术分析和预研。\n岗位要求\n1、本科及以上学历，计算机，电子信息，通信，自动化、机械、数学、物理等相关理工科背景；\n2、有意愿从事嵌入式软开方向，与业务一起成长；\n3、熟练掌握C/C++语言，熟悉linux操作系统，了解基本网络知识，操作系统原理等；\n4、具有极强的沟通能力及解决问题的能力，良好的团队合作精神，抗压能力。\n\n1、本科及以上学历，计算机、软件工程、电子信息等相关专业优先；\n2、熟练掌握C/C++编程语言，熟悉面向对象编程思维，有良好的编程习惯；\n3、具备网络、多线程、数据库、QT、Python等一种或若干种经验的优先；\n4、有较强的质量意识、学习能力和对技术的热爱，具备良好的文档阅读和编写能能力；\n5、有责任心，具备良好的沟通、表达能力和团队协作精神。','633487'),(14,'上海宽德投资管理中心','高性能计算工程师','20-40K * 16薪','上海/珠海','岗位职责\n高性能计算工程师，探索不同硬件平台上的高性能计算技术研究，结合量化行业的计算需求特征，打造领先的高性能推理平台，为交易提供高效可靠的计算服务。\n岗位职责：\n● 研究CPU/GPU等硬件平台上的高性能计算技术，参与高性能计算库、推理引擎的开发与优化。\n● 探索基于计算图分析的高性能算子自动生成与优化方法。\n● 与硬件团队联合，进行软硬协调设计，探索异构加速的极致方案。\n● 探索各种前沿技术，持续改进相关系统的性能。\n\n岗位要求\n●  全日制重点高校本科及以上学历，计算机、软件工程、电子工程等理工科相关专业。\n●  掌握C/C++/Python中的一种或多种语言。\n●  扎实掌握计算机体系结构、数据结构和算法等基础。\n●  享受编程，享受创造，具有强烈的好奇心与追求极致的态度。\n\n加分项：（非必选项，能有一项或多项符合者优先）\n● 熟悉算法优化，并在相关算法竞赛活动中获得优秀成绩优先。\n● 熟悉特定体系(CPU/GPU)的性能优化，并有实际优化经验优先。\n● 熟悉一种或多种并行编程语言优先，如MPI/OpenMP/OpenCL/CUDA等。\n● 熟悉机器学习在线推理，有TensorRT、OpenVINO相关经验优先。\n● 熟悉TVM等机器学习编译器的开发，并有相关经验。',' 132-1111-2222');
/*!40000 ALTER TABLE `joblist` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `questionlist`
--

DROP TABLE IF EXISTS `questionlist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `questionlist` (
  `number` int(11) NOT NULL AUTO_INCREMENT COMMENT '题号',
  `title` varchar(128) NOT NULL COMMENT '题目标题',
  `difficulty` varchar(8) NOT NULL COMMENT '题目难度',
  `describe` text NOT NULL COMMENT '题目描述',
  `headContent` text NOT NULL COMMENT '题目头部，用户初始代码',
  `tailContent` text NOT NULL COMMENT '题目尾部，包含测试用例',
  `CpuLimit` int(11) DEFAULT '1' COMMENT '时间限制',
  `MemoryLimit` int(11) DEFAULT '262144' COMMENT '空间限制',
  PRIMARY KEY (`number`)
) ENGINE=InnoDB AUTO_INCREMENT=18 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `questionlist`
--

LOCK TABLES `questionlist` WRITE;
/*!40000 ALTER TABLE `questionlist` DISABLE KEYS */;
INSERT INTO `questionlist` VALUES (1,'回文数','Easy','给你一个整数 x ，如果 x 是一个回文整数，返回 true ；否则，返回 false 。\n\n回文数是指正序（从左向右）和倒序（从右向左）读都是一样的整数。\n\n例如，121 是回文，而 123 不是。\n\n\n示例 1：\n\n输入：x = 121\n输出：true\n示例 2：\n\n输入：x = -121\n输出：false\n解释：从左向右读, 为 - 121 。 从右向左读, 为 121 - 。因此它不是一个回文数。\n示例 3：\n\n输入：x = 10\n输出：false\n解释：从右向左读, 为 01 。因此它不是一个回文数。','#include <iostream>\n#include <string>\n#include <vector>\n#include <map>\n#include <algorithm>\nusing namespace std;\n\nclass Solution\n{\npublic:\n    bool IsPalindrome(int x) {\n        //write code\n        return true;\n    }\n};','bool Test1()\n{\n    bool ret = Solution().IsPalindrome(121);\n    if (ret) {\n        return true;\n    }\n    else {\n        std::cout << \"未通过测试用例1 : 121\" << std::endl;\n        return false;\n    }\n}\nbool Test2()\n{\n    bool ret = Solution().IsPalindrome(-10);\n    if (!ret) {\n        return true;\n    }\n    else {\n        std::cout << \"未通过测试用例1 : -10\" << std::endl;\n        return false;\n    }\n}\n\nint main()\n{\n    int ret1 = Test1();\n    int ret2 = Test2();\n    if (ret1 && ret2) {\n        std::cout << \"通过全部测试用例\" << endl;\n    }\n    return 0;\n}',2,262144),(2,'测试1','Easy','测试','暂无','无',1,1),(3,'测试2','Medium','测试','暂无','无',2,2),(4,'测试3','Medium','测试','暂无','无',3,3),(5,'测试4','Easy','测试','暂无','无',4,4),(6,'测试5','Hard','测试','暂无','无',5,5),(7,'测试6','Hard','测试','暂无','无',1,1),(8,'测试7','Hard','测试','暂无','无',1,1),(9,'测试8','Hard','测试','暂无','无',1,1),(16,'跳跃游戏','Medium','给定一个非负整数数组 nums ，你最初位于数组的 第一个下标 。\n\n数组中的每个元素代表你在该位置可以跳跃的最大长度。\n\n判断你是否能够到达最后一个下标。','#include <iostream>\n#include <vector>\n#include <string>\n#include <map>\nusing namespace std;\n\nclass Solution {\npublic:\n    bool canJump(vector<int>& nums) {\n\n    }\n};','bool Test1() {\n    vector<int> nums{ 2,3,1,1,4 };\n    bool tmp = Solution().canJump(nums);\n    if (tmp == true) return true;\n    else {\n        std::cout << \"测试用例 [2,3,1,1,4] 未通过\" << std::endl;\n        return false;\n    }\n}\n\nbool Test2() {\n    vector<int> nums{ 3,2,1,0,4 };\n    bool tmp = Solution().canJump(nums);\n    if (tmp == false) return true;\n    else {\n        std::cout << \"测试用例 [3,2,1,0,4] 未通过\" << std::endl;\n        return false;\n    }\n}\n\nint main() \n{\n    bool ret1 = Test1();\n    bool ret2 = Test2();\n    if (ret1 && ret2) {\n        std::cout << \"通过全部测试用例\" << endl;\n    }\n    return 0;\n}',1,262144);
/*!40000 ALTER TABLE `questionlist` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user`
--

DROP TABLE IF EXISTS `user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user` (
  `UserId` int(11) NOT NULL AUTO_INCREMENT COMMENT '用户ID',
  `user` varchar(32) NOT NULL COMMENT '用户名',
  `password` int(11) NOT NULL COMMENT '密码',
  PRIMARY KEY (`UserId`),
  UNIQUE KEY `user` (`user`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user`
--

LOCK TABLES `user` WRITE;
/*!40000 ALTER TABLE `user` DISABLE KEYS */;
INSERT INTO `user` VALUES (1,'tzq',12345678),(3,'bjy',666666),(9,'lsj',372207098),(11,'example',123);
/*!40000 ALTER TABLE `user` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2023-08-08 22:42:13
