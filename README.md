# 지능형 영상처리(Intelligent-Image-Processing)

## 1. 과제들-Assignments

### (1) 개요

이 과목을 수강하면서 만들었던 프로젝트들이다. 프로젝트들은 MFC또는 Open-CV를 이용했다. 과목을 수강하면서 영상처리에 대한 다양한 알고리즘과 방법들을 배웠다.



### (2) 과제들

#### 1) 컨볼류션 마스크를 이용한 영상 영역처리

![image](https://user-images.githubusercontent.com/20302410/52468376-eca36f00-2bcb-11e9-97f5-46c9e74a4647.png)

#### 2) 차선 검출 및 허프만 실습

![image](https://user-images.githubusercontent.com/20302410/52468470-25434880-2bcc-11e9-92e2-232208cdd409.png)

#### (3) 최소자승법을 응용한 불균일한 배경영상 밝기값 보정하기

![image](https://user-images.githubusercontent.com/20302410/52468604-91be4780-2bcc-11e9-83d7-3057eb1fbf73.png)

## 2. 텀프로젝트-term project

### (1) 개요

Dynamic edge tolerance algorithm, AB Compare algorithm을 이용하여 패턴 영상의 결함을 찾고, 원본 영상의 결함 부분에 마크를 표시하는 프로젝트이다. 

영상 이진화, 이진 닫힘 모폴로지 방법도 사용하였다. 



### (2) 실행 결과

#### 1) 원본 영상

![image](https://user-images.githubusercontent.com/20302410/52468151-2758d780-2bcb-11e9-8c23-81ec8096634c.png)

원본 영상의 크기가 너무 커서 화면에 다 나오지 않아 결함이 있는 부분만 따로 편집해 사용하였다.

#### 2) Dynamic edge tolerance algorithm을 적용하여 차영상 구하기

![image](https://user-images.githubusercontent.com/20302410/52468154-29bb3180-2bcb-11e9-93f2-85f10f2421b4.png)

원래 결함이 있는 부분 좌측 패턴에 Ghost Defect가 생긴 것을 볼 수 있었다. 

#### 3) AB Compare 알고리즘을 사용하여 Ghost Defect 제거하기

![image](https://user-images.githubusercontent.com/20302410/52468157-2b84f500-2bcb-11e9-84e6-aa23c58ef215.png)

좌측 패턴과 AND 연산을 한 결과이다. Ghost Defect가 있던 부분을 사라지고, 원래 결함이 있는 부분만 남은 것을 확인할 수 있었다.

#### 4) 영상 이진화하고 이진 닫힘 모폴로지 연산하기

![image](https://user-images.githubusercontent.com/20302410/52468162-2d4eb880-2bcb-11e9-8248-00d4d7fa7a26.png)

위의 영상 결과를 이진화 하고, 3번 팽창, 2번 침식한 결과이다. 결함과 관계없는 노이즈 부분은 사라지고, 결함 부분이 뚜렷하게 나온 것을 볼 수 있었다.

#### 5) Defect 정보 획득하고 검사영상에 Defect 위치 표시하기

![image](https://user-images.githubusercontent.com/20302410/52468165-317ad600-2bcb-11e9-9208-165ed2f72dc0.png)

원본 영상과 Defect 추적 결과 영상이다. 결함 부분 주위로 하얀색 사각형이 그려졌고, 결함의 중심 좌표를 출력하였다. 



