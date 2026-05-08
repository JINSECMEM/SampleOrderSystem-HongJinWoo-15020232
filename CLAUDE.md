# CLAUDE.md

Claude Code가 이 프로젝트에서 작업할 때 참고하는 가이드 문서입니다.

## 프로젝트 개요

반도체 시료 생산 관리 시스템 — C++ 콘솔 앱 (Visual Studio, MSVC, C++17)

- **솔루션**: `SampleOrderSystem.slnx`
- **진입점**: `SampleOrderSystem\SampleOrderSystem.cpp`
- **개발자**: jinwoo8.hong (사번: 15020232)

## 아키텍처

POC `ConsoleMVC_gitlocal` 기반 **MVC + Repository + Service Layer** 구조를 따른다.

```
SampleOrderSystem/
├── Models/          # 도메인 모델 (struct, enum)
├── Views/           # 콘솔 입출력 (메뉴, 화면)
├── Controllers/     # 흐름 제어 (AppController 포함)
├── Services/        # 비즈니스 로직
├── Database/        # 데이터 접근 계층
└── Utils/           # 공용 유틸리티 (ConsoleHelper 등)
```

## 도메인 핵심 개념

### 주문 상태 흐름

```
RESERVED → REJECTED          (거절)
RESERVED → CONFIRMED          (승인 + 재고 충분)  → RELEASE
RESERVED → PRODUCING → CONFIRMED                  → RELEASE
```

| 상태 | 의미 |
|------|------|
| `RESERVED` | 주문 접수, 승인 대기 중 |
| `REJECTED` | 주문 거절 |
| `PRODUCING` | 승인 완료, 재고 부족으로 생산 중 |
| `CONFIRMED` | 승인 완료, 출고 대기 중 |
| `RELEASE` | 출고 완료 |

### 주요 엔티티

- **Sample**: 시료 (id, name, avg_production_time, yield_rate, stock)
- **Order**: 주문 (id, sample_id, quantity, status, timestamps)
- **ProductionJob**: 생산 작업 (order_id, target_qty, produced_qty, status)
- **Inventory**: 재고 (sample_id, quantity)

### 생산량 공식

```cpp
int actual_qty = std::ceil(shortage / (yield_rate * 0.9));
int total_time = avg_production_time * actual_qty;
```

## 데이터 영속성

POC `DataPersist` 패턴 참고 — JSON 또는 CSV 파일 기반으로 런타임 데이터를 저장한다.
파일 위치: `data/` 디렉터리 (실행 파일 기준 상대 경로).

## 빌드 환경

- IDE: Visual Studio 2022
- 언어: C++17 (`/std:c++17`)
- 플랫폼: Windows x64
- 인코딩: UTF-8 (`SetConsoleOutputCP(CP_UTF8)`)

## 커밋 메시지 템플릿

```
[Description] <변경 내용 한 줄 요약>
[Type] design | imple | refactor | test | bugfix
[Developer] jinwoo8.hong
```

## 테스트

**Test Harness**: Google Mock (gmock) — Visual Studio에서 직접 추가
- 단위 테스트는 `Tests/` 프로젝트(혹은 디렉터리)에 작성한다.
- Mock 대상: Repository, Service 계층의 인터페이스

## 참고 POC 경로

| POC | 경로 | 참고 내용 |
|-----|------|-----------|
| MVC 스켈레톤 | `C:\reviewer\ConsoleMVC_gitlocal` | 전체 구조, DI, Database 패턴 |
| 데이터 영속성 | `C:\reviewer\DataPersist\Project1` | 파일 저장/로드, `std::optional` |
| 데이터 모니터링 | `C:\reviewer\DataMonitoring\DataMonitor-HongJinWoo-15020232` | 콘솔 테이블 포맷팅(`iomanip`), 정렬 조회, 메뉴 루프 패턴 |
| 더미 데이터 생성 | `C:\reviewer\DummyDataGen` | 테스트 데이터, enum 변환 |
