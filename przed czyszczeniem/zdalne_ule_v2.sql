-- phpMyAdmin SQL Dump
-- version 5.2.0
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Czas generowania: 03 Lip 2022, 15:53
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
-- Baza danych: `zdalne_ule_v2`
--

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `esp-master`
--

CREATE TABLE `esp-master` (
  `ID` int(11) NOT NULL,
  `lokacja` text NOT NULL,
  `nazwa` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Zrzut danych tabeli `esp-master`
--

INSERT INTO `esp-master` (`ID`, `lokacja`, `nazwa`) VALUES
(1, 'runowo krajeńskie', 'esp01'),
(2, 'wałdowo', 'esp02');

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `esp-master-zmiany`
--

CREATE TABLE `esp-master-zmiany` (
  `ID` int(11) NOT NULL,
  `ID_esp-master` int(11) NOT NULL,
  `data_zmiany` datetime NOT NULL DEFAULT current_timestamp(),
  `nowa_nazwa` text NOT NULL,
  `nowa_lokacja` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `esp-slave`
--

CREATE TABLE `esp-slave` (
  `ID` int(11) NOT NULL,
  `ID_esp-master` int(11) NOT NULL,
  `nr_ula` int(11) NOT NULL,
  `nazwa` text DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Zrzut danych tabeli `esp-slave`
--

INSERT INTO `esp-slave` (`ID`, `ID_esp-master`, `nr_ula`, `nazwa`) VALUES
(0, 1, 10, 'ulA'),
(1, 1, 11, 'ulB'),
(2, 2, 20, 'ulC');

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `pomiary_pojedyncze`
--

CREATE TABLE `pomiary_pojedyncze` (
  `ID` int(11) NOT NULL,
  `ID_pomiar_zbiorowy` int(11) NOT NULL,
  `ID_esp_slave` int(11) NOT NULL,
  `waga` decimal(6,2) NOT NULL,
  `temperatura` decimal(6,2) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Zrzut danych tabeli `pomiary_pojedyncze`
--

INSERT INTO `pomiary_pojedyncze` (`ID`, `ID_pomiar_zbiorowy`, `ID_esp_slave`, `waga`, `temperatura`) VALUES
(1, 9, 0, '50.00', '35.00'),
(2, 10, 0, '55.00', '38.00'),
(3, 9, 1, '35.00', '25.00'),
(4, 10, 1, '45.00', '35.00'),
(5, 11, 2, '50.00', '55.00'),
(6, 12, 2, '60.00', '25.00');

-- --------------------------------------------------------

--
-- Struktura tabeli dla tabeli `pomiary_zbiorowe`
--

CREATE TABLE `pomiary_zbiorowe` (
  `ID_pomiar_zbiorowy` int(11) NOT NULL,
  `ID_esp-master` int(11) NOT NULL,
  `data` datetime NOT NULL DEFAULT current_timestamp(),
  `temperatura_zewn` decimal(6,2) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Zrzut danych tabeli `pomiary_zbiorowe`
--

INSERT INTO `pomiary_zbiorowe` (`ID_pomiar_zbiorowy`, `ID_esp-master`, `data`, `temperatura_zewn`) VALUES
(9, 1, '2022-07-02 20:39:09', '25.00'),
(10, 1, '2022-07-02 21:39:14', '22.00'),
(11, 2, '2022-07-02 20:39:50', '15.00'),
(12, 2, '2022-07-02 21:39:52', '10.00');

--
-- Indeksy dla zrzutów tabel
--

--
-- Indeksy dla tabeli `esp-master`
--
ALTER TABLE `esp-master`
  ADD PRIMARY KEY (`ID`);

--
-- Indeksy dla tabeli `esp-master-zmiany`
--
ALTER TABLE `esp-master-zmiany`
  ADD PRIMARY KEY (`ID`),
  ADD KEY `ID_esp-master` (`ID_esp-master`);

--
-- Indeksy dla tabeli `esp-slave`
--
ALTER TABLE `esp-slave`
  ADD PRIMARY KEY (`ID`),
  ADD KEY `ID_esp-master` (`ID_esp-master`);

--
-- Indeksy dla tabeli `pomiary_pojedyncze`
--
ALTER TABLE `pomiary_pojedyncze`
  ADD PRIMARY KEY (`ID`),
  ADD KEY `ID_pomiar_zbiorowy` (`ID_pomiar_zbiorowy`),
  ADD KEY `ID_esp-slave` (`ID_esp_slave`);

--
-- Indeksy dla tabeli `pomiary_zbiorowe`
--
ALTER TABLE `pomiary_zbiorowe`
  ADD PRIMARY KEY (`ID_pomiar_zbiorowy`),
  ADD KEY `ID_esp-master` (`ID_esp-master`);

--
-- AUTO_INCREMENT dla zrzuconych tabel
--

--
-- AUTO_INCREMENT dla tabeli `esp-master`
--
ALTER TABLE `esp-master`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;

--
-- AUTO_INCREMENT dla tabeli `esp-master-zmiany`
--
ALTER TABLE `esp-master-zmiany`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT dla tabeli `pomiary_pojedyncze`
--
ALTER TABLE `pomiary_pojedyncze`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;

--
-- AUTO_INCREMENT dla tabeli `pomiary_zbiorowe`
--
ALTER TABLE `pomiary_zbiorowe`
  MODIFY `ID_pomiar_zbiorowy` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=13;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
