% UDP的MATLAB实现
% u1是用来接收服务器端的端口，'192.168.0.108'改成服务端侧的IP地址
% u2是用来发送给可视化的端口，'192.168.0.107'改成可视化端的IP地址，RemotePort改为可视化端的端口号，LocalPort无关紧要，随便设置个不重复的即可
u1=udp('127.0.0.1','RemotePort',8089,'LocalPort',8854);
u1.InputBufferSize = 4096;  %字节数
u2=udp('127.0.0.1','RemotePort',8854,'LocalPort',8845); % 传输端口 '192.168.0.108'填远程主机的ip
fopen(u1);%打开udp连接（实际上并没有连接，udp是无连接的通信协议）
fopen(u2);

fprintf(u1,'p'); % 主机1（就是udp1）发送文本消息
recv = fread(u1,4096,'uchar');
recv_info = recv(1:2:end,:);
% recv_head = recv_info(1,1);
% 通过标识符判断包是否正确
% if(recv_head ~= 1)
%     fclose(u1);delete(u1);clear u1;
%     error('package接受错误，请重新运行程序!');
% end
recv_info_no_head = recv_info(1:end,:);

agent = struct('path_length',0, 'Speed', 0,'LastSpeed',0, 'LocationX',0,'LocationY', 0, 'Direction', 0, 'path_array', 0,...
                'path_passed_length', 1, 'path_x', [], 'path_y', [], 'path_dir', []);

disp('agent initializing...')
position_path_length(1) = 1;
agent(1).path_length = recv_info_no_head(position_path_length(1), 1);

% agent initializing...

agent(1).Speed = 0;

agent(1).LastSpeed = 0;

agent(1).path_passed_length = 1;

agent(1).LocationX = recv_info_no_head(position_path_length(1)+1, 1);

agent(1).LocationY = recv_info_no_head(position_path_length(1)+2, 1);

agent(1).Direction = recv_info_no_head(position_path_length(1)+3, 1);

agent(1).path_array = recv_info_no_head(position_path_length(1)+1:end, 1);

agent(1).path_x = agent(1).path_array(1:3:end,1);
agent(1).path_y = agent(1).path_array(2:3:end,1);
agent(1).path_dir = agent(1).path_array(3:3:end,1);

disp("agent Initialized finished");

%PID控制小车循路径规划前进
pid = struct('SetSpeed', 0, 'LastSpeed', 0, 'err', 0.0, 'err_last',0.0, 'acceleration', 0.0, 'integral', 0.0,...
            'Kp', 0.2, 'Ki', 0.015, 'Kd', 0.2);
pid_i = 1;

pid(pid_i).SetSpeed = 0;
pid(pid_i).LastSpeed = 0;
pid(pid_i).err = 0.0;
pid(pid_i).err_last = 0.0;
pid(pid_i).acceleration = 0.0;
pid(pid_i).integral = 0.0;
pid(pid_i).Kp = 0.2;
pid(pid_i).Ki = 0.015;
pid(pid_i).Kd = 0.2;

disp('pid Initialized finished \n');

for i = 1:1
    while(1)
        j = 1;
        along_count = 0;
        dir_last = agent(i).path_dir(agent(i).path_passed_length, 1);
        while(j<=5 && (agent(i).path_passed_length+j)<agent(i).path_length)
            if(agent(i).path_dir(agent(i).path_passed_length+j,1)==dir_last)
                along_count = along_count + 1;
            else
                break;
            end
            j = j + 1;
        end
        % 自己规定几个速度
        %1. 转弯速度是1格/s
        %2. 最大直线速度是5格/s（暂定）

        % 举例：方向，看五个格子，
        % 前面5格全是同一方向 SetSpeed = 5
        % 前面4格全是同一方向 SetSpeed = 4
        % 前面3格全是同一方向 SetSpeed = 3
        % 前面2格全是同一方向 SetSpeed = 2
        % 前面1格全是同一方向 SetSpeed = 1
        % 前面1格是转弯       SetSpeed = 1
        % 其余情况下          SetSpeed = 1(防止停下)
        switch along_count
            case 0
                %下一格就转弯 Speed = 1
                agent(i).Speed = 1;
            case 1
                %还有一格直行，Speed = 1
                agent(i).Speed = 1;
            case 2
                %还有2格直行，Speed = 2
                agent(i).Speed = 2;
            case 3
                agent(i).Speed = 3;
            case 4
                agent(i).Speed = 4;
            case 5
                % 司机的最大视野，也是最大速度
                %还有5格直行，Speed = 5
                agent(i).Speed = 5;
            otherwise
                agent(i).Speed = 1;
        end

        % 有了目标Speed后，可以通过PID调节速度，
        % 通过path_passed_length控制数组下标，用来判断来到了第几个格子。
        % 下一步的方向就按照自己（path_dir(path_passed_length,1)）的方向走
        pid_temp = PID_realize(pid(i), agent(i).Speed, agent(i).LastSpeed);
        agent(i).Speed = agent(i).LastSpeed + pid_temp.acceleration;
        agent(i).LastSpeed = agent(i).Speed;
        
        if((agent(i).path_passed_length + agent(i).Speed)<agent(i).path_length)
            agent(i).path_passed_length = agent(i).path_passed_length + agent(i).Speed;
        else
            agent(i).path_passed_length = agent(i).path_length;
        end
        
        % 发送在这里改
        send_msg(1) = num2str(agent(i).Speed);
        send_msg(2) = num2str(agent(i).path_dir(agent(i).path_passed_length, 1));
        % 模拟向远程发送
        fprintf(u2,send_msg);
        % 模拟远程接受
%         fscanf(u1);
        
%         disp(num2str(agent(i).path_passed_length));
        pause(10);
        if(agent(i).path_passed_length == agent(i).path_length)
            display_str = ['agent', num2str(i),' achieved!'];
            disp(display_str);
            break;
        end
    end
end

%关闭udp1连接
%删除udp1连接，释放内存
%清除工作区中的udp1数据
fclose(u1);delete(u1);clear u1;
fclose(u2);delete(u2);clear u2;
