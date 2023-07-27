close all

%waga kalibracja 
podstawka = (412+282)*1e-3;
kloc1 = 1823e-3;
kloc1i2 = 2706e-3;

x = [
    0;
    podstawka;
    podstawka+kloc1;
    podstawka+kloc1i2;
    ];

y1 = [
    976600;%nic
    994000;%podstawka
    1070000; %kloc1
    1119000;%kloc2
    ];

plot(x,y1)
%%%

a = 4.199e+04; %41990
b = 9.705e+05; %970500

%%
pomiar = 50;
odp = (pomiar-b)/a
