clear
close all
table = readtable("pomiary.csv");
table2 = sortrows(table,"timeFromDB","ascend");

noGsmSleep = table2(15:27,:);
noGsmSleep.czasOdPoczatku = seconds(noGsmSleep.timeFromDB - noGsmSleep.timeFromDB(1) )/3600

gsmSleep = table2(42:57,:);
gsmSleep.czasOdPoczatku = seconds(gsmSleep.timeFromDB - gsmSleep.timeFromDB(1) )/3600

gsmSleepNoLed = table2(58:73,:);
gsmSleepNoLed.czasOdPoczatku = seconds(gsmSleepNoLed.timeFromDB - gsmSleepNoLed.timeFromDB(1) )/3600

plot(noGsmSleep,"czasOdPoczatku","bateria","Color","red")
hold on
plot(gsmSleep,"czasOdPoczatku","bateria","Color","blue")
plot(gsmSleepNoLed,"czasOdPoczatku","bateria","Color","green")
legend("sleep tylko na esp","sleep na esp i gsm","sleep na esp,gsm i wylutowanie leda")
