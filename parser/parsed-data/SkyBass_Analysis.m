if ~(exist('bmp') && exist('mma'))
    bmp = csvread('bmp.csv');
    mma = csvread('mma.csv');
end

START = 714000;
LEN = 5700;
APO = 892;

p = (bmp(:,2) + bmp(:,3))/2;
h = (1-((p)./101350).^0.190284) .* 145366.45;  %%in ft
h = h(START:START+LEN) - h(START);


%% Acceleration analysis
START = 714000;
LEN = 5700;
APO = 892;


time_a = mma(START:START+LEN,1);
time_a = (time_a - time_a(1))/1000000;
ax = mma(START:START+LEN,2);
ay = mma(START:START+LEN,3);


%clean up bad samples
for i = 1:LEN
    if (ax(i) < -400) || (ay(i) < -400)
        ax(i) = (ax(i-1)+ax(i+1))/2;
        ay(i) = (ay(i-1)+ay(i+1))/2;
    end
end

v_dr = zeros(APO,1);
h_dr = zeros(APO,1);
for i = 2:APO
    v_dr(i) = v_dr(i-1) + -(ay(i)+ay(i+1))/2 * (time_a(i+1) - time_a(i));
    h_dr(i) = h_dr(i-1) + (v_dr(i-1)+v_dr(i))/2 * (time_a(i+1) - time_a(i));
end
h_dr = h_dr * 3.28084;  %% convert to ft
v_dr = v_dr * 3.28084;  %% convert to ft/s
ay = ay / 9.81;   %% convert to g's  

clf
hold on;
%yyaxis right;
%plot(time_a(1:APO),v_dr);
%ylabel('Velocity (ft/s)')
%yyaxis left;
plot(time_a(1:APO),h_dr);
plot(time_a,h)
ylabel('Height (ft)');
ylim([-100,6000])
xlabel('Time (s)');
title('Skybass Dead Reckoning vs Barrometer')