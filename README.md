

## IRC Server

3인 팀 프로젝트  
C++, TCP 기반 실시간 채팅 서버  

## 이 프로젝트를 왜 했는가?
http 서버를 만드는 프로젝트가 아닌 이 프로젝트를 선택했습니다.  
만들고 싶은 게임은 실시간 게임이고, 이에 따라 IRC 서버 프로젝트를 선택하게 되었습니다.  


## 환경

운영체제 : Ubuntu  
언어/표준 : C++ (C++98, 요구사항)  
빌드 : GNU Make (요구 사항)  
코딩 표준 : [POCU C++](https://docs.popekim.com/ko/coding-standards/pocu-cpp)  

## 핵심 기능   
- [O] 커맨드 공통 인터페이스(CommonCommand) + 확장 클래스로 각 명령 모듈화    
- [O] 바이트 스트림 파서 : CRLF 기준 분할, 프리픽스/명령/매개변수 토크나이즈  (각 명령어에서 파싱)  
- [O] 세션/로비/채널 모델    
- [O] 명령어 기능 구현: PASS, NICK, USER, PART, QUIK (본인 구현)/ JOIN, TOPIC, MODE, KICK, INVITE, PRIVMSG (팀원 구현)  
- [O] 에러 핸들링 : 인증 실패(잘못된 비밀번호 , 오랜 로그인 시간), 커맨드 오류(잘못된 명령어 또는 포맷)  
- [O] 간단한 봇 구현

## 실행

서버 실행  
make  
./ircserv 6667 <password>


https://github.com/user-attachments/assets/60a9d745-d6e6-4fba-a552-e3a95d8b8099


irssi 설치 방법  
sudo apt update  
sudo apt install -y irssi  



## 명령어 처리 흐름
1. 수신 버퍼 누적(바이트 스트림)  
2. 메시지 경계 탐지: \r\n 기준 분리   
3. 명령어에 따라 다형적인 팩토리 메서드 패턴으로 로비, 채널 등에서 커맨드 인스턴스를 생성  
4. 만들어진 인스턴스의 로직을 실행하여 messages를 반환하거나 채널 로직 실행  
5. 반환된 messages는 클라이언트로 수신하여 처리  

### 프로토콜 호환

기준 문서: RFC [2811](https://datatracker.ietf.org/doc/html/rfc2811), [2812](https://datatracker.ietf.org/doc/html/rfc2812)  
  
제한/가정:   
- 메시지 길이(512B)  
- 닉네임 규칙 : 1 ~ 30자 이내의 숫자 및 알파벳  
- 채널 이름 규칙   

### 리뷰  
1. 포맷터를 따로 만들고, 커맨드 인스턴스가 에러 코드를 반환하게 하여 에러 코드에 따라 행동했으면 좋았을거라 생각  
2. 서버 쪽에 로그 시스템이 있으면 좋을 거 같음.
3. 팩토리 메서드에서 map을 활용하는 것이 나았을 거 같다.
