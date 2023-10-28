-- phpMyAdmin SQL Dump
-- version 5.2.0
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Czas generowania: 31 Lip 2023, 20:21
-- Wersja serwera: 10.4.24-MariaDB
-- Wersja PHP: 8.1.6

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Baza danych: `zdalne_ule_v3`
--

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `esp_devices`
--

CREATE TABLE `esp_devices` (
  `ID` int(11) NOT NULL,
  `nazwa` varchar(16) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Zrzut danych tabeli `esp_devices`
--

INSERT INTO `esp_devices` (`ID`, `nazwa`) VALUES
(0, 'esp01');

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `pomiary`
--

CREATE TABLE `pomiary` (
  `ID` int(10) UNSIGNED NOT NULL,
  `espID` int(11) NOT NULL,
  `waga` decimal(10,2) NOT NULL,
  `timeFromEsp` datetime DEFAULT NULL,
  `timeFromDB` datetime NOT NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Zrzut danych tabeli `pomiary`
--

INSERT INTO `pomiary` (`ID`, `espID`, `waga`, `timeFromEsp`, `timeFromDB`) VALUES
(1, 0, '69.42', '2023-06-11 13:43:34', '2023-06-11 13:46:31'),
(2, 0, '69.42', '2023-06-11 13:43:34', '2023-06-11 14:14:53'),
(3, 0, '8388607.00', '2023-06-11 14:12:10', '2023-06-11 14:14:54'),
(4, 0, '8388607.00', '2023-06-11 14:12:42', '2023-06-11 14:15:26'),
(5, 0, '8388607.00', '2023-06-11 14:13:15', '2023-06-11 14:15:59'),
(6, 0, '8388607.00', '2023-06-11 14:13:48', '2023-06-11 14:16:32'),
(7, 0, '8388607.00', '2023-06-11 14:14:20', '2023-06-11 14:17:04'),
(8, 0, '-8388608.00', '2023-06-11 14:14:53', '2023-06-11 14:17:38'),
(9, 0, '1912.00', '2023-06-11 14:15:27', '2023-06-11 14:18:11'),
(10, 0, '1910.00', '2023-06-11 14:16:03', '2023-06-11 14:18:47'),
(11, 0, '-7135767.00', '2023-06-11 14:16:37', '2023-06-11 14:19:21'),
(12, 0, '-8388608.00', '2023-06-11 14:17:10', '2023-06-11 14:19:54'),
(13, 0, '-8388608.00', '2023-06-11 14:17:43', '2023-06-11 14:20:27'),
(14, 0, '-8388608.00', '2023-06-11 14:18:16', '2023-06-11 14:21:00'),
(15, 0, '-8388608.00', '2023-06-11 14:18:50', '2023-06-11 14:21:35'),
(16, 0, '-8388608.00', '2023-06-11 14:19:24', '2023-06-11 14:22:08'),
(17, 0, '-8388608.00', '2023-06-11 14:19:58', '2023-06-11 14:22:43'),
(18, 0, '-8388608.00', '2023-06-11 14:20:33', '2023-06-11 14:23:17'),
(19, 0, '-8388608.00', '2023-06-11 14:21:05', '2023-06-11 14:23:50'),
(20, 0, '5.42', '2023-07-27 13:43:34', '2023-07-27 18:16:31'),
(22, 0, '5.42', '2023-07-27 14:43:34', '2023-07-27 18:17:18');

--
-- Indeksy dla zrzutów tabel
--

--
-- Indeksy dla tabeli `esp_devices`
--
ALTER TABLE `esp_devices`
  ADD PRIMARY KEY (`ID`);

--
-- Indeksy dla tabeli `pomiary`
--
ALTER TABLE `pomiary`
  ADD PRIMARY KEY (`ID`);

--
-- AUTO_INCREMENT dla zrzuconych tabel
--

--
-- AUTO_INCREMENT dla tabeli `pomiary`
--
ALTER TABLE `pomiary`
  MODIFY `ID` int(10) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=23;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
