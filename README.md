# SimpleDxInputExample

Simple DirectInput example

## How To Use
1. Create
```c++
  std::unique_ptr<GInput> pInputInstance = std::unique_ptr<GInput>(new GInput(HWND, int, int));
```

2. Update
```c++
  GInput::Update();
```

3. Release(Choose one of the methods here)
```c++
  GInput::Release();
```

```c++
  pInputInstance->ShutDown();
```

```c++
  // pInputInstance release
```

4. Use

```c++
  GInput::Get()->IsKeyDown(GKEY_DEF_0); // keyboard input
  GInput::Get()->IsMouseDown(GMOUSE_BUTTONS_LEFT); // mouse input
```
