%% 生成四种失真波形，并将其保存
clc;clear;close all;
t = 0:0.01:4*pi; threshold = 1.0;
y_none = 2*sin(t); % 设置无失真(为虚线)
y_up = y_none; y_up(y_up > threshold) = threshold; % 设置为顶部失真
y_down = y_none; y_down(y_down < -threshold) = -threshold; % 设置为底部失真
y_bi = y_down; y_bi(y_bi > threshold) = threshold; % 设置为双向失真


%% 绘制图形
figure(1);
subplot(221); plot(t, y_none, 'k', 'LineWidth',2); grid on; 
xlabel('time'); ylabel('amplitude'); title('没有失真');
xlim([-0.5, 13]); ylim([-2.5,2.5]);

subplot(222); plot(t, y_none, 'r:', 'LineWidth',1.8); grid on;
hold on; plot(t, y_up, 'r', 'LineWidth',2); grid on;
xlabel('time'); ylabel('amplitude'); title('顶部失真');
xlim([-0.5, 13]); ylim([-2.5,2.5]);

subplot(223); plot(t, y_none, 'g:', 'LineWidth',1.8); grid on;
hold on; plot(t, y_down, 'g', 'LineWidth',2); grid on;
xlabel('time'); ylabel('amplitude'); title('底部失真');
xlim([-0.5, 13]); ylim([-2.5,2.5]);

subplot(224); plot(t, y_none, 'b:', 'LineWidth',1.8); grid on;
hold on; plot(t, y_bi, 'b', 'LineWidth',2); grid on;
xlabel('time'); ylabel('amplitude'); title('双向失真');
xlim([-0.5, 13]); ylim([-2.5,2.5]);
