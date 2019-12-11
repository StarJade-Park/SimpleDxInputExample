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

3. Check input

```c++
  if( GInput::Get()->IsKeyDown(GKEY_DEF_0) ) // keyboard input
  {
    // ...
  }
  
  if( GInput::Get()->IsMouseDown(GMOUSE_BUTTONS_LEFT) ) // mouse input
  {
    // ...
  }
```

4. Release(Choose one of the methods here)

```c++
  // pInputInstance release
```

```c++
  GInput::Release();
```

```c++
  pInputInstance->ShutDown();
```
