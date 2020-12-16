# Data_structure_File_explorer
##자료구조실습 7조 프로젝트

      파일 탐색기 제작

<br/>
## 컴파일 방법
   
      g++ main.cpp sort.cpp makedirectory.cpp makefile.cpp delete.cpp gotoxy.cpp move.cpp rename.cpp -o main.out -std=c++14

<br/>
## 프로그램 이용 방법:
<br/>
<br/>
## 1. Normal mode

      w: 위로 커서 이동

      s: 아래로 커서 이동

      a: 커서가 위의 메뉴에 있으면 왼쪽으로 커서 이동

      d: 커서가 위의 메뉴에 있으면 오른쪽으로 커서 이동

      q: 프로그램 종료

      z: Zip mode로 변경 

      x: 압축 해제

      m: Move mode로 변경

      r: 이름 변경(누르면 새로운 파일 이름 작성 후 이름 변경)

      enter: 디렉토리 이동(커서가 디렉토리에 있을 경우) , 메뉴 선택(커서가 위의 메뉴에 있을 경우)

<br/>
## 2. Zip mode(위아래 커서는 동일)

      p: 압축할 파일 추가

      z: 추가한 파일 압축(누르면 압축 파일 이름 작성 후 압축)

      q: Zip mode 종료

<br/>
## 3. Move mode(위아래 커서는 동일)

      p: 이동할 파일 추가

      m: 추가한 파일 이동(누르면 경로 작성 후 이동)

      q: Move mode 종료

<br/>
## 4. 위의 메뉴
<br/>
<br/>
#### Sort
      
      1. Name 선택 시 이름순으로 파일 정렬
      
      2. Size 선택 시 파일 크기순으로 파일 정렬
      
      3. Time 선택 시 파일의 최근 사용 시간순으로 정렬
      
      (모두 기본 값은 오름차순 정렬이지만, 한번더 enter 입력시 내림차순 정렬)

      4. w, s, q 입력시 벗어날수 있음

#### Delete: 파일 삭제 기능(선택 시 삭제할 파일 이름 작성 후 파일 삭제) 

#### File: 파일 생성 기능(선택 시 생성할 파일 이름과 간단한 텍스트를 작성후 파일 생성)

#### Directory: 디렉토리 생성 기능(선택시 생성할 디렉토리 이름 작성후 디렉토리 생성)
