[33mcommit 4e7cacbc56d1838514d3c0497cabaa40cc928986[m[33m ([m[1;36mHEAD -> [m[1;32mmain[m[33m)[m
Author: ahskal <kkj380377@gmail.com>
Date:   Sat Sep 9 22:11:42 2023 +0900

    GameObject.cpp 컬러 상수버퍼 추가
    셰이더에서 읽을수있게 상수버퍼 바인딩
    
    GameObjectGUI.cpp GUI색 변경 코드 추가
    GameObjectSaveLoad.cpp 세이브오브젝트에서 color저장부분 추가
    GameObjectSaveLoad.cpp 로드오브젝트에서 color로드부분 추가
    
    쉐이더
    Common.hlsl 컬러 상수버퍼 추가
    6.Cube.hlsl 컬러값 가져와서 계산식 추가
    -> 기존 2D에서 사용하던 색적용방식이랑 동일

[33mcommit b1a9793eaafec081edc222db4ab4293fe0a77221[m
Author: Carnissa <34364273+ahskal@users.noreply.github.com>
Date:   Sat Sep 9 22:05:41 2023 +0900

    Initial commit
