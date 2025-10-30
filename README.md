

## IRC Server

3인 팀 프로젝트  
C++ — RFC 기반 메시지 파싱과 커맨드 공통 인터페이스로 모듈화한 IRC 서버  

## 이 프로젝트를 왜 했는가?
http 서버를 만드는 프로젝트가 아닌 이 프로젝트를 선택했습니다.  
만들고 싶은 게임은 실시간 게임이고, 이에 따라 IRC 서버 프로젝트를 선택하게 되었습니다.  


## 구현 목록   
- [DETAIL_0_ENVIRONMENT_AND_DIRECTION](docs/DETAIL_0_ENVIRONMENT_AND_DIRECTION.md)  
    - 개발 환경 및 방향  
- [DETAIL_1_COMMUNICATION](docs/DETAIL_1_COMMUNICATION.md)  
    - 클라이언트와 서버 간의 통신  
- [DETAIL_2_MODELING](docs/DETAIL_2_MODELING.md)  
    - 로그인/로비/채널/서버/유저  
- [DETAIL_3_COMMAND](docs/DETAIL_3_PROTOCOL_AND_COMMAND.md)  
    - 커맨드 공통 인터페이스(CommonCommand) + 확장 클래스로 각 명령 모듈화   
    - 명령어 기능 구현: PASS, NICK, USER, PART, QUIK (본인 구현)  
    - JOIN, TOPIC, MODE, KICK, INVITE, PRIVMSG (팀원 구현)  
- [DETAIL_4_HANDLE_ERROR](docs/DETAIL_4_HANDLE_ERROR.md)  
    - 에러 핸들링 : 인증 실패(잘못된 비밀번호 , 오랜 로그인 시간), 커맨드 오류(잘못된 명령어 또는 포맷)  
- [DETAIL_5_BOT](docs/DETAIL_5_BOT.md)  
    - 봇 구현  

## 실행

서버 실행  
make  
./ircserv 6667 <password>


https://github.com/user-attachments/assets/60a9d745-d6e6-4fba-a552-e3a95d8b8099


irssi 설치 방법  
sudo apt update  
sudo apt install -y irssi  
