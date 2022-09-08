function [pid] = PID_realize(pid,speed,feedbackspeed)
	pid.SetSpeed = speed;
	pid.LastSpeed = feedbackspeed;
	pid.err = pid.SetSpeed-pid.LastSpeed;
	pid.integral = pid.integral + pid.err;
	pid.acceleration = round(pid.Kp.*pid.err+pid.Ki.*pid.integral+pid.Kd.*(pid.err-pid.err_last)+0.5);
	pid.err_last = pid.err; 
    % 限制最大油门
    if(pid.acceleration > 2)
        pid.acceleration = 2;
    end
    pid.LastSpeed = pid.LastSpeed + pid.acceleration;
end

