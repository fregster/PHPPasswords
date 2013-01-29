-- phpMyAdmin SQL Dump
-- version 2.11.8.1deb5+lenny6
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Dec 04, 2010 at 09:26 PM
-- Server version: 5.0.51
-- PHP Version: 5.2.6-1+lenny9

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `phppasswords`
--

-- --------------------------------------------------------

--
-- Table structure for table `tbl_archive`
--

CREATE TABLE IF NOT EXISTS `tbl_archive` (
  `id` int(11) NOT NULL,
  `table` varchar(32) NOT NULL,
  `data` text NOT NULL,
  `date` int(11) NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `tbl_group`
--

CREATE TABLE IF NOT EXISTS `tbl_group` (
  `gid` int(11) NOT NULL,
  `name` varchar(32) NOT NULL,
  `description` varchar(128) default NULL,
  PRIMARY KEY  (`gid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `tbl_password`
--

CREATE TABLE IF NOT EXISTS `tbl_password` (
  `id` int(11) NOT NULL,
  `data` text NOT NULL,
  `date` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `tbl_record`
--

CREATE TABLE IF NOT EXISTS `tbl_record` (
  `id` int(11) NOT NULL,
  `title` varchar(128) NOT NULL,
  `service` varchar(64) NOT NULL,
  `tags` varchar(254) default NULL,
  `link` varchar(254) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `tbl_settings`
--

CREATE TABLE IF NOT EXISTS `tbl_settings` (
  `setting` varchar(64) NOT NULL,
  `value` varchar(128) NOT NULL,
  PRIMARY KEY  (`setting`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `tbl_users`
--

CREATE TABLE IF NOT EXISTS `tbl_users` (
  `id` int(11) NOT NULL auto_increment,
  `username` varchar(64) NOT NULL,
  `password` varchar(128) NOT NULL,
  `salt` varchar(128) NOT NULL,
  `email` varchar(128) NOT NULL,
  `name` varchar(64) default NULL,
  `admin` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `username` (`username`),
  KEY `uname` (`username`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=2 ;

-- --------------------------------------------------------

--
-- Table structure for table `tbl_user_group_membership`
--

CREATE TABLE IF NOT EXISTS `tbl_user_group_membership` (
  `uid` int(11) NOT NULL,
  `gid` int(11) NOT NULL,
  UNIQUE KEY `uid` (`uid`,`gid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
