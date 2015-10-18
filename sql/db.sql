CREATE DATABASE reactors;

use reactors;


-- CREATE USER 'reactors'@'%' IDENTIFIED BY 'Dfl2cR38prF2vbT';

-- GRANT ALL ON reactors.* TO 'reactors'@'%';

-- phpMyAdmin SQL Dump
-- version 3.5.1
-- http://www.phpmyadmin.net
--
-- Хост: localhost
-- Время создания: Окт 18 2015 г., 17:46
-- Версия сервера: 5.5.33-log
-- Версия PHP: 5.3.15

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- База данных: `reactors`
--

-- --------------------------------------------------------

--
-- Структура таблицы `Lab`
--

CREATE TABLE IF NOT EXISTS `Lab` (
  `LabID` int(11) NOT NULL AUTO_INCREMENT,
  `SchemaID` int(11) NOT NULL,
  `UserID` int(11) NOT NULL,
  `LabDateTime` datetime NOT NULL,
  `LabComment` varchar(255) COLLATE utf8mb4_bin DEFAULT NULL,
  PRIMARY KEY (`LabID`),
  UNIQUE KEY `LabID` (`LabID`),
  KEY `LabID_2` (`LabID`),
  KEY `SchemaID` (`SchemaID`),
  KEY `UserID` (`UserID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Структура таблицы `Parameter`
--

CREATE TABLE IF NOT EXISTS `Parameter` (
  `ParameterID` int(11) NOT NULL AUTO_INCREMENT,
  `ParameterName` varchar(127) COLLATE utf8mb4_bin NOT NULL,
  `ParameterUnits` varchar(10) COLLATE utf8mb4_bin NOT NULL,
  PRIMARY KEY (`ParameterID`),
  KEY `ParameterID` (`ParameterID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

-- --------------------------------------------------------

--
-- Структура таблицы `Point`
--

CREATE TABLE IF NOT EXISTS `Point` (
  `PointID` int(11) NOT NULL AUTO_INCREMENT,
  `ParameterID` int(11) NOT NULL,
  `LabID` int(11) NOT NULL,
  `PointTime` DOUBLE NOT NULL,
  `PointValue` double NOT NULL,
  PRIMARY KEY (`PointID`),
  KEY `PointID` (`PointID`),
  KEY `ParameterID` (`ParameterID`),
  KEY `LabID` (`LabID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Структура таблицы `Schema`
--

CREATE TABLE IF NOT EXISTS `Schema` (
  `SchemaID` int(11) NOT NULL AUTO_INCREMENT,
  PRIMARY KEY (`SchemaID`),
  KEY `SchemaID` (`SchemaID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Структура таблицы `User`
--

CREATE TABLE IF NOT EXISTS `User` (
  `UserID` int(11) NOT NULL AUTO_INCREMENT,
  `UserName` varchar(127) COLLATE utf8mb4_bin NOT NULL,
  `UserPassword` varchar(127) COLLATE utf8mb4_bin NOT NULL,
  PRIMARY KEY (`UserID`),
  UNIQUE KEY `UserName` (`UserName`),
  KEY `UserID` (`UserID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin AUTO_INCREMENT=1 ;

--
-- Ограничения внешнего ключа сохраненных таблиц
--

--
-- Ограничения внешнего ключа таблицы `Lab`
--
ALTER TABLE `Lab`
  ADD CONSTRAINT `Lab_ibfk_1` FOREIGN KEY (`SchemaID`) REFERENCES `Schema` (`SchemaID`),
  ADD CONSTRAINT `Lab_ibfk_2` FOREIGN KEY (`UserID`) REFERENCES `User` (`UserID`);

--
-- Ограничения внешнего ключа таблицы `Point`
--
ALTER TABLE `Point`
  ADD CONSTRAINT `Point_ibfk_2` FOREIGN KEY (`LabID`) REFERENCES `Lab` (`LabID`) ON DELETE CASCADE,
  ADD CONSTRAINT `Point_ibfk_1` FOREIGN KEY (`ParameterID`) REFERENCES `Parameter` (`ParameterID`);
  
  
INSERT INTO `User` (UserName, UserPassword) VALUES ('admin','admin');
INSERT INTO `Schema` VALUES ();
INSERT INTO `Parameter` (ParameterName, ParameterUnits) VALUES ('Conductivity', 'mS/cm');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
