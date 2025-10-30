

## 통신

클라이언트끼리 통신하기 위해 만든 서버에 접속해야 합니다.  

이를 위해 서버는 소켓을 열어 클라이언트의 접속을 기다렸습니다.  

접속된 클라이언트의 데이터를 수신하여 로직을 처리합니다.

<details>
    <summary>call stack</summary>
커맨드 라인 인자로 포트번호와 비밀번호를 받고, 서버를 생성합니다.    
https://github.com/SeJin0214/IRCServer/blob/5e9b3328d729d1fa6e18429474a3e23c9a54547c/main.cpp#L27
<br>

IPv4와 TCP 프로토콜을 사용하여 서버 소켓을 생성합니다.  
서버 소켓은 클라이언트의 연결 요청을 수락하기 위한 용도로 사용됩니다.  
https://github.com/SeJin0214/IRCServer/blob/5e9b3328d729d1fa6e18429474a3e23c9a54547c/core/Server.cpp#L52
<br>

Port 번호와 IP 주소를 설정해줍니다.  
https://github.com/SeJin0214/IRCServer/blob/5e9b3328d729d1fa6e18429474a3e23c9a54547c/core/Server.cpp#L66-L71
<br>

클라이언트의 접속 요청을 기다립니다.  
https://github.com/SeJin0214/IRCServer/blob/5e9b3328d729d1fa6e18429474a3e23c9a54547c/core/Server.cpp#L78
<br>

종료 조건이 될 때까지 서버를 실행합니다.  
https://github.com/SeJin0214/IRCServer/blob/5e9b3328d729d1fa6e18429474a3e23c9a54547c/main.cpp#L28
https://github.com/SeJin0214/IRCServer/blob/5e9b3328d729d1fa6e18429474a3e23c9a54547c/core/Server.cpp#L382-L384
<br>

멀티플렉싱 IO 함수인 select를 이용해 소켓 이벤트를 감지합니다.
https://github.com/SeJin0214/IRCServer/blob/5e9b3328d729d1fa6e18429474a3e23c9a54547c/core/Server.cpp#L386-L392
<br>

1. 새로운 연결 요청을 처리합니다.
https://github.com/SeJin0214/IRCServer/blob/5e9b3328d729d1fa6e18429474a3e23c9a54547c/core/Server.cpp#L397-L405
https://github.com/SeJin0214/IRCServer/blob/5e9b3328d729d1fa6e18429474a3e23c9a54547c/core/Server.cpp#L433-L445
<br>

2. 클라이언트의 데이터 수신(바이트 스트림)을 처리합니다.
https://github.com/SeJin0214/IRCServer/blob/5e9b3328d729d1fa6e18429474a3e23c9a54547c/core/Server.cpp#L397
https://github.com/SeJin0214/IRCServer/blob/5e9b3328d729d1fa6e18429474a3e23c9a54547c/core/Server.cpp#L451
https://github.com/SeJin0214/IRCServer/blob/5e9b3328d729d1fa6e18429474a3e23c9a54547c/core/Server.cpp#L491


</details>

