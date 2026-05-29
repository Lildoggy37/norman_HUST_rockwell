%% 真空中环形载流线圈电磁场数值模拟与可视化


%% ====================== 一、参数定义模块 ======================
clear all; close all; clc;

% --------------------- 物理参数 ---------------------
mu0 = 4*pi*1e-7;          % 真空磁导率 [H/m]，默认值：4π×10^-7
I = 1.0;                  % 线圈电流 [A]，建议范围：0.1-100 A
R = 0.1;                  % 线圈半径 [m]，建议范围：0.01-1 m
N = 1;                    % 线圈匝数，默认：1

% --------------------- 计算空间参数 ---------------------
% 三维空间范围 [m]
x_range = [-0.3, 0.3];    % x轴范围，建议：±(2-3)R
y_range = [-0.3, 0.3];    % y轴范围，建议：±(2-3)R
z_range = [-0.3, 0.3];    % z轴范围，建议：±(2-3)R

% 网格分辨率
grid_points = 31;         % 每轴网格点数，建议：21-51（根据内存调整）
                         % 注意：点数过大会显著增加计算时间

% --------------------- 可视化参数 ---------------------
plot_3D_vector = true;    % 是否绘制三维矢量图
plot_2D_slice = true;     % 是否绘制二维截面图
plot_iso_surface = true;  % 是否绘制三维等值面图
plot_contour = true;      % 是否绘制等高线图

% 场强显示阈值（避免极端值影响可视化）
B_max_threshold = 1e-4;   % 最大磁感应强度阈值 [T]，默认：1e-4
B_min_threshold = 1e-8;   % 最小磁感应强度阈值 [T]，默认：1e-8

% 绘图颜色映射
color_map = 'jet';        % 颜色方案：'jet', 'parula', 'hot', 'cool', 'hsv'

% 图像保存设置
save_images = true;       % 是否保存图像
image_format = 'png';     % 图像格式：'png', 'jpg', 'pdf', 'fig'
image_dpi = 300;          % 图像分辨率 [DPI]

% 线圈离散参数（数值积分精度）
coil_segments = 100;      % 线圈离散段数，建议：50-200

%% ====================== 二、输入参数校验 ======================
fprintf('=== 参数校验 ===\n');

% 检查物理参数合理性
if R <= 0
    error('线圈半径R必须为正数');
end
if I == 0
    warning('电流I为0，电磁场计算结果将为0');
end
if grid_points < 10
    error('网格点数过少，建议至少为10');
end

% 检查内存需求（预估）
estimated_memory = (grid_points^3 * 24) / (1024^2); % MB
if estimated_memory > 500
    warning('预估内存需求：%.1f MB。考虑减小grid_points或使用稀疏网格。', estimated_memory);
end

fprintf('参数校验通过，开始计算...\n\n');

%% ====================== 三、网格生成 ======================
fprintf('=== 生成计算网格 ===\n');

% 创建三维网格
x = linspace(x_range(1), x_range(2), grid_points);
y = linspace(y_range(1), y_range(2), grid_points);
z = linspace(z_range(1), z_range(2), grid_points);
[X, Y, Z] = meshgrid(x, y, z);

% 记录网格点总数
total_points = numel(X);
fprintf('网格生成完成：%dx%dx%d = %d个计算点\n', ...
    grid_points, grid_points, grid_points, total_points);

%% ====================== 四、线圈几何定义 ======================
% 生成线圈离散点（在x-y平面上的圆）
theta_coil = linspace(0, 2*pi, coil_segments + 1);
theta_coil = theta_coil(1:end-1); % 避免重复点

% 线圈上各点的坐标
coil_x = R * cos(theta_coil);
coil_y = R * sin(theta_coil);
coil_z = zeros(size(theta_coil)); % 线圈在z=0平面

% 线圈上各点的电流微元方向（切线方向）
% 对于圆形线圈，电流方向沿切线：dl = R*dθ*(-sinθ, cosθ, 0)
dl_x = -R * sin(theta_coil) * (2*pi/coil_segments);
dl_y = R * cos(theta_coil) * (2*pi/coil_segments);
dl_z = zeros(size(theta_coil));

%% ====================== 五、电磁场计算模块 ======================
fprintf('=== 开始电磁场计算 ===\n');
fprintf('计算进度：');

% 初始化磁场分量矩阵
Bx = zeros(size(X));
By = zeros(size(X));
Bz = zeros(size(X));

% 初始化进度显示
progress_step = floor(coil_segments/10);
if progress_step == 0, progress_step = 1; end

% 基于毕奥-萨伐尔定律计算每个网格点的磁场
% 公式：dB = (μ0/4π) * I * (dl × r) / |r|^3
for i = 1:coil_segments
    % 显示计算进度
    if mod(i, progress_step) == 0
        fprintf('%d%% ', round(i/coil_segments*100));
    end
    
    % 当前线圈微元的位置
    coil_pos = [coil_x(i), coil_y(i), coil_z(i)];
    dl = [dl_x(i), dl_y(i), dl_z(i)];
    
    % 计算从线圈微元到所有网格点的矢量r
    % 避免循环，使用向量化计算
    rx = X - coil_pos(1);
    ry = Y - coil_pos(2);
    rz = Z - coil_pos(3);
    
    % 计算距离r的三次方
    r_mag = sqrt(rx.^2 + ry.^2 + rz.^2);
    r_cubed = r_mag.^3;
    
    % 避免分母为零（线圈上的点）
    r_cubed(r_cubed < eps) = eps;
    
    % 计算叉积 dl × r
    cross_x = dl(2)*rz - dl(3)*ry;
    cross_y = dl(3)*rx - dl(1)*rz;
    cross_z = dl(1)*ry - dl(2)*rx;
    
    % 应用毕奥-萨伐尔定律累加贡献
    constant = (mu0 * I * N) / (4 * pi);
    Bx = Bx + constant * cross_x ./ r_cubed;
    By = By + constant * cross_y ./ r_cubed;
    Bz = Bz + constant * cross_z ./ r_cubed;
end

fprintf('100%%\n');

% 计算总磁感应强度大小
B_mag = sqrt(Bx.^2 + By.^2 + Bz.^2);

% 应用场强阈值限制（用于可视化）
B_mag_vis = B_mag;
B_mag_vis(B_mag_vis > B_max_threshold) = B_max_threshold;
B_mag_vis(B_mag_vis < B_min_threshold) = B_min_threshold;

fprintf('电磁场计算完成！\n');
fprintf('最大磁感应强度：%.2e T\n', max(B_mag(:)));
fprintf('最小磁感应强度：%.2e T\n', min(B_mag(:)));

%% ====================== 六、可视化模块 ======================
fprintf('\n=== 生成可视化图像 ===\n');

% 设置默认绘图属性
set(0, 'DefaultFigureColor', 'white');
set(0, 'DefaultAxesFontSize', 11);
set(0, 'DefaultTextFontSize', 12);

% 创建图形文件夹
if save_images && ~exist('output_figures', 'dir')
    mkdir('output_figures');
end

%% 1. 二维截面图（x-z平面，y=0）
if plot_2D_slice
    fprintf('生成二维截面图...\n');
    
    fig1 = figure('Position', [100, 100, 800, 600], 'Name', '二维截面分布');
    
    % 提取y=0截面的索引
    [~, y0_idx] = min(abs(y - 0));
    
    % 创建子图布局
    subplot(2, 2, 1);
    imagesc(x*100, z*100, squeeze(B_mag_vis(:, y0_idx, :))');
    axis equal tight;
    xlabel('x [cm]'); ylabel('z [cm]');
    title('磁感应强度大小 |B| (x-z平面)');
    colorbar; colormap(color_map);
    set(gca, 'YDir', 'normal');
    
    % 添加线圈位置
    hold on;
    plot(R*100, 0, 'ro', 'MarkerSize', 8, 'LineWidth', 2);
    plot(-R*100, 0, 'ro', 'MarkerSize', 8, 'LineWidth', 2);
    hold off;
    
    % 各分量分布
    subplot(2, 2, 2);
    imagesc(x*100, z*100, squeeze(Bx(:, y0_idx, :))');
    xlabel('x [cm]'); ylabel('z [cm]');
    title('B_x分量 (x-z平面)');
    colorbar; colormap(color_map);
    set(gca, 'YDir', 'normal');
    
    subplot(2, 2, 3);
    imagesc(x*100, z*100, squeeze(By(:, y0_idx, :))');
    xlabel('x [cm]'); ylabel('z [cm]');
    title('B_y分量 (x-z平面)');
    colorbar; colormap(color_map);
    set(gca, 'YDir', 'normal');
    
    subplot(2, 2, 4);
    imagesc(x*100, z*100, squeeze(Bz(:, y0_idx, :))');
    xlabel('x [cm]'); ylabel('z [cm]');
    title('B_z分量 (x-z平面)');
    colorbar; colormap(color_map);
    set(gca, 'YDir', 'normal');
    
    sgtitle(sprintf('环形载流线圈电磁场分布 (I=%.1fA, R=%.2fm)', I, R));
    
    if save_images
        saveas(fig1, sprintf('output_figures/2D_slice_%.0fA_%.2fm.%s', I, R, image_format));
        fprintf('  图像已保存\n');
    end
end

%% 2. 等高线图（叠加在线圈平面）
if plot_contour
    fprintf('生成等高线图...\n');
    
    fig2 = figure('Position', [200, 200, 700, 500], 'Name', '等高线分布');
    
    % 提取z=0平面（线圈平面）
    [~, z0_idx] = min(abs(z - 0));
    B_plane = squeeze(B_mag(:,:,z0_idx))';
    
    % 创建等高线图
    contourf(x*100, y*100, B_plane, 20, 'LineStyle', 'none');
    hold on;
    
    % 添加线圈轮廓
    theta = linspace(0, 2*pi, 100);
    plot(R*100*cos(theta), R*100*sin(theta), 'w-', 'LineWidth', 2);
    
    % 添加磁场矢量箭头（抽样显示）
    skip = 3; % 抽样间隔
    quiver(x(1:skip:end)*100, y(1:skip:end)*100, ...
           squeeze(Bx(1:skip:end,1:skip:end,z0_idx))', ...
           squeeze(By(1:skip:end,1:skip:end,z0_idx))', ...
           1.5, 'k', 'LineWidth', 0.5);
    
    xlabel('x [cm]'); ylabel('y [cm]');
    title('线圈平面(z=0)磁感应强度等高线');
    colorbar; colormap(color_map);
    axis equal tight;
    grid on;
    
    if save_images
        saveas(fig2, sprintf('output_figures/contour_%.0fA_%.2fm.%s', I, R, image_format));
        fprintf('  图像已保存\n');
    end
end

%% 3. 三维矢量图
if plot_3D_vector
    fprintf('生成三维矢量图...\n');
    
    fig3 = figure('Position', [300, 300, 900, 700], 'Name', '三维矢量场');
    
    % 抽样显示，避免过于密集
    skip = max([1, floor(grid_points/10)]);
    X_sub = X(1:skip:end, 1:skip:end, 1:skip:end);
    Y_sub = Y(1:skip:end, 1:skip:end, 1:skip:end);
    Z_sub = Z(1:skip:end, 1:skip:end, 1:skip:end);
    Bx_sub = Bx(1:skip:end, 1:skip:end, 1:skip:end);
    By_sub = By(1:skip:end, 1:skip:end, 1:skip:end);
    Bz_sub = Bz(1:skip:end, 1:skip:end, 1:skip:end);
    B_mag_sub = sqrt(Bx_sub.^2 + By_sub.^2 + Bz_sub.^2);
    
    % 三维箭头图
    quiver3(X_sub*100, Y_sub*100, Z_sub*100, ...
            Bx_sub, By_sub, Bz_sub, 1.5, ...
            'LineWidth', 0.8, 'MaxHeadSize', 0.5);
    
    hold on;
    
    % 绘制线圈
    plot3(coil_x*100, coil_y*100, coil_z*100, ...
          'r-', 'LineWidth', 3);
    
    % 设置图形属性
    xlabel('x [cm]'); ylabel('y [cm]'); zlabel('z [cm]');
    title(sprintf('三维磁场矢量分布 (I=%.1fA, R=%.2fm)', I, R));
    grid on; box on;
    view(35, 30); % 设置视角
    axis equal tight;
    
    % 添加色标映射（通过伪色彩）
    caxis([min(B_mag_sub(:)), max(B_mag_sub(:))]);
    colormap(color_map);
    colorbar;
    
    if save_images
        saveas(fig3, sprintf('output_figures/3D_vector_%.0fA_%.2fm.%s', I, R, image_format));
        fprintf('  图像已保存\n');
    end
end

%% 4. 三维等值面图
if plot_iso_surface
    fprintf('生成三维等值面图...\n');
    
    fig4 = figure('Position', [400, 400, 850, 650], 'Name', '三维等值面');
    
    % 选择合适的等值面值
    iso_values = linspace(min(B_mag(:))*1.1, max(B_mag(:))*0.5, 3);
    
    % 绘制等值面
    for i = 1:length(iso_values)
        isovalue = iso_values(i);
        patch(isosurface(X*100, Y*100, Z*100, B_mag, isovalue), ...
              'FaceColor', [i/length(iso_values), 0.5, 1-i/length(iso_values)], ...
              'EdgeColor', 'none', ...
              'FaceAlpha', 0.6 - 0.2*(i-1), ...
              'DisplayName', sprintf('|B| = %.2e T', isovalue));
        hold on;
    end
    
    % 绘制线圈
    plot3(coil_x*100, coil_y*100, coil_z*100, ...
          'k-', 'LineWidth', 3, 'DisplayName', '载流线圈');
    
    % 设置图形属性
    xlabel('x [cm]'); ylabel('y [cm]'); zlabel('z [cm]');
    title(sprintf('磁感应强度等值面 (I=%.1fA, R=%.2fm)', I, R));
    grid on; box on;
    view(45, 30); % 设置视角
    axis equal tight;
    legend('show', 'Location', 'best');
    lighting gouraud;
    light('Position', [1 1 1], 'Style', 'infinite');
    camlight('headlight');
    material('dull');
    
    if save_images
        saveas(fig4, sprintf('output_figures/isosurface_%.0fA_%.2fm.%s', I, R, image_format));
        fprintf('  图像已保存\n');
    end
end

%% ====================== 七、结果保存模块 ======================
fprintf('\n=== 保存计算结果 ===\n');

% 保存数据到MAT文件
data_filename = sprintf('magnetic_field_data_I%.0fA_R%.2fm.mat', I, R);
save(data_filename, 'X', 'Y', 'Z', 'Bx', 'By', 'Bz', 'B_mag', ...
     'mu0', 'I', 'R', 'N', 'x_range', 'y_range', 'z_range', 'grid_points');
fprintf('计算数据已保存至：%s\n', data_filename);

% 生成参数报告
report_filename = sprintf('simulation_report_I%.0fA_R%.2fm.txt', I, R);
fid = fopen(report_filename, 'w');
fprintf(fid, '=== 环形载流线圈电磁场模拟报告 ===\n\n');
fprintf(fid, '模拟时间：%s\n', datestr(now));
fprintf(fid, '\n一、物理参数\n');
fprintf(fid, '  线圈电流 I = %.2f A\n', I);
fprintf(fid, '  线圈半径 R = %.3f m\n', R);
fprintf(fid, '  线圈匝数 N = %d\n', N);
fprintf(fid, '  真空磁导率 μ₀ = %.3e H/m\n', mu0);
fprintf(fid, '\n二、计算空间\n');
fprintf(fid, '  x范围: [%.3f, %.3f] m\n', x_range(1), x_range(2));
fprintf(fid, '  y范围: [%.3f, %.3f] m\n', y_range(1), y_range(2));
fprintf(fid, '  z范围: [%.3f, %.3f] m\n', z_range(1), z_range(2));
fprintf(fid, '  网格点数: %d×%d×%d = %d\n', ...
    grid_points, grid_points, grid_points, total_points);
fprintf(fid, '\n三、计算结果\n');
fprintf(fid, '  最大磁感应强度: %.3e T\n', max(B_mag(:)));
fprintf(fid, '  最小磁感应强度: %.3e T\n', min(B_mag(:)));
fprintf(fid, '  平均磁感应强度: %.3e T\n', mean(B_mag(:)));
fprintf(fid, '\n四、生成图像\n');
if plot_2D_slice, fprintf(fid, '  ✓ 二维截面图\n'); end
if plot_contour, fprintf(fid, '  ✓ 等高线图\n'); end
if plot_3D_vector, fprintf(fid, '  ✓ 三维矢量图\n'); end
if plot_iso_surface, fprintf(fid, '  ✓ 三维等值面图\n'); end
fclose(fid);

fprintf('参数报告已保存至：%s\n', report_filename);

%% ====================== 八、完成提示 ======================
fprintf('\n=== 模拟完成！ ===\n');
fprintf('总计算点数：%d\n', total_points);
fprintf('计算时间记录：请使用Matlab的tic/toc功能记录具体时间\n');
fprintf('图像保存在output_figures文件夹中\n');
fprintf('数据保存在%s中\n', data_filename);

% 显示主要结果摘要
fprintf('\n--- 结果摘要 ---\n');
fprintf('在线圈中心点(0,0,0)：\n');
center_idx = [floor(grid_points/2)+1, floor(grid_points/2)+1, floor(grid_points/2)+1];
B_center = [Bx(center_idx(1), center_idx(2), center_idx(3)), ...
            By(center_idx(1), center_idx(2), center_idx(2)), ...
            Bz(center_idx(1), center_idx(2), center_idx(3))];
fprintf('  B = [%.3e, %.3e, %.3e] T\n', B_center(1), B_center(2), B_center(3));
fprintf('  |B| = %.3e T\n', sqrt(sum(B_center.^2)));

% 理论值比较（单匝圆形线圈轴线上的磁场）
% 轴线点(0,0,z)处的理论值：B_z = (μ0*I*R^2)/(2*(R^2+z^2)^(3/2))
if abs(R) > 0
    z_test = 0.1; % 测试点
    B_theory = (mu0*I*R^2)/(2*(R^2+z_test^2)^(1.5));
    fprintf('\n理论验证（轴线点z=%.2fm）：\n', z_test);
    fprintf('  理论值 B_z = %.3e T\n', B_theory);
    
    % 查找对应的数值计算结果
    [~, z_idx] = min(abs(z - z_test));
    B_num = Bz(center_idx(1), center_idx(2), z_idx);
    fprintf('  数值解 B_z = %.3e T\n', B_num);
    fprintf('  相对误差 = %.2f%%\n', abs((B_num-B_theory)/B_theory)*100);
end