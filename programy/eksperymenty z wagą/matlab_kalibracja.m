close all

%waga kalibracja 
ciezarek = 4850e-3; %kg
slownik = 1481e-3;
kuchnia = 1385e-3;
sherlock = 1752e-3;
podstawka = 1069e-3;

x = [
    0;
    podstawka;
    podstawka+sherlock;
    podstawka+sherlock+kuchnia;
    podstawka+sherlock+kuchnia+slownik;
    podstawka+sherlock+kuchnia+slownik+ciezarek;
    podstawka+sherlock+kuchnia+slownik+ciezarek+ciezarek
    ];

y1 = [
    846000;
    870900;
    911400;
    943500;
    977200;
    1088680;
    1202305
    ];
y2 = [
    846000;
    870500;
    910800;
    943020;
    976800;
    1087850;
    1200750
    ];

y_avg = mean(y1,y2)

% plot(y1);
% hold on
% plot(y2);
%plot(x,y_avg)

% y = ax+b
%x = (y-b)/a
odp = (863000-b)/a
