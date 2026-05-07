#ifndef CAMERA_INDEX_H
#define CAMERA_INDEX_H

static const char camera_index_html[] PROGMEM = R"HTML(
<!doctype html>
<html lang="ru">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32-CAM Vision Deck</title>
  <script defer src="https://cdn.jsdelivr.net/npm/@tensorflow/tfjs@4.22.0/dist/tf.min.js"></script>
  <script defer src="https://cdn.jsdelivr.net/npm/@tensorflow-models/coco-ssd@2.2.3/dist/coco-ssd.min.js"></script>
  <script defer src="https://cdn.jsdelivr.net/npm/@tensorflow-models/hand-pose-detection@2.1.0/dist/hand-pose-detection.min.js"></script>
  <script defer src="https://cdn.jsdelivr.net/npm/face-api.js@0.22.2/dist/face-api.min.js"></script>
  <style>
    :root {
      --bg: #07141c;
      --bg-soft: #102634;
      --panel: rgba(8, 25, 36, 0.78);
      --panel-strong: rgba(10, 30, 43, 0.9);
      --line: rgba(149, 197, 214, 0.18);
      --text: #eaf6ff;
      --muted: #9eb8c4;
      --accent: #41d4b7;
      --accent-2: #ffb35c;
      --danger: #ff7171;
      --shadow: 0 24px 80px rgba(0, 0, 0, 0.36);
      --radius: 22px;
    }

    * {
      box-sizing: border-box;
    }

    body {
      margin: 0;
      min-height: 100vh;
      font-family: "Aptos", "Bahnschrift", "Segoe UI", sans-serif;
      color: var(--text);
      background:
        radial-gradient(circle at top left, rgba(65, 212, 183, 0.16), transparent 34%),
        radial-gradient(circle at top right, rgba(255, 179, 92, 0.13), transparent 30%),
        linear-gradient(160deg, #041018 0%, #0b1d29 52%, #06121a 100%);
    }

    .layout {
      width: min(1500px, calc(100% - 28px));
      margin: 18px auto 28px;
      display: grid;
      gap: 18px;
    }

    .hero,
    .card {
      background: var(--panel);
      border: 1px solid var(--line);
      border-radius: var(--radius);
      box-shadow: var(--shadow);
      backdrop-filter: blur(18px);
    }

    .hero {
      position: relative;
      overflow: hidden;
      padding: 24px;
      display: grid;
      gap: 18px;
    }

    .hero::after {
      content: "";
      position: absolute;
      inset: auto -12% -36% auto;
      width: 340px;
      height: 340px;
      background: radial-gradient(circle, rgba(65, 212, 183, 0.22), transparent 62%);
      pointer-events: none;
    }

    .hero-top {
      display: flex;
      justify-content: space-between;
      gap: 12px;
      align-items: flex-start;
      flex-wrap: wrap;
    }

    .eyebrow {
      display: inline-flex;
      align-items: center;
      gap: 10px;
      padding: 8px 12px;
      border-radius: 999px;
      color: var(--muted);
      background: rgba(255, 255, 255, 0.04);
      border: 1px solid rgba(255, 255, 255, 0.06);
      font-size: 13px;
      letter-spacing: 0.03em;
      text-transform: uppercase;
    }

    h1 {
      margin: 14px 0 10px;
      font-family: "Bahnschrift", "Trebuchet MS", sans-serif;
      font-size: clamp(28px, 4.2vw, 54px);
      line-height: 0.98;
      letter-spacing: 0.01em;
    }

    .hero-copy {
      max-width: 760px;
      color: var(--muted);
      line-height: 1.55;
      font-size: 15px;
    }

    .hero-meta,
    .chip-row,
    .toolbar,
    .toggle-grid,
    .slider-grid,
    .stat-grid,
    .face-list {
      display: flex;
      gap: 10px;
      flex-wrap: wrap;
    }

    .chip {
      display: inline-flex;
      align-items: center;
      gap: 8px;
      padding: 10px 14px;
      border-radius: 999px;
      border: 1px solid rgba(255, 255, 255, 0.07);
      background: rgba(255, 255, 255, 0.04);
      color: var(--text);
      font-size: 13px;
    }

    .chip strong {
      color: #ffffff;
      font-weight: 700;
    }

    .content {
      display: grid;
      gap: 18px;
      grid-template-columns: minmax(0, 1.55fr) minmax(330px, 0.95fr);
    }

    .card {
      padding: 18px;
    }

    .stage-card {
      padding: 18px;
      display: grid;
      gap: 14px;
    }

    .stage-wrap {
      position: relative;
      overflow: hidden;
      min-height: 300px;
      border-radius: 18px;
      border: 1px solid rgba(255, 255, 255, 0.08);
      background:
        linear-gradient(180deg, rgba(3, 11, 16, 0.18), rgba(3, 11, 16, 0.35)),
        repeating-linear-gradient(135deg, rgba(255, 255, 255, 0.03) 0 14px, rgba(255, 255, 255, 0.01) 14px 28px),
        #05111a;
      aspect-ratio: 16 / 9;
    }

    #stream {
      width: 100%;
      height: 100%;
      object-fit: contain;
      display: block;
      background: #02080c;
    }

    #overlay {
      position: absolute;
      inset: 0;
      width: 100%;
      height: 100%;
      pointer-events: none;
    }

    .stage-hud {
      position: absolute;
      inset: 14px 14px auto 14px;
      display: flex;
      justify-content: space-between;
      gap: 12px;
      flex-wrap: wrap;
      pointer-events: none;
    }

    .stage-badge {
      display: inline-flex;
      align-items: center;
      gap: 8px;
      padding: 9px 12px;
      border-radius: 999px;
      background: rgba(0, 0, 0, 0.38);
      color: #f2fbff;
      border: 1px solid rgba(255, 255, 255, 0.08);
      font-size: 12px;
      letter-spacing: 0.02em;
    }

    .stage-note {
      position: absolute;
      left: 14px;
      bottom: 14px;
      max-width: calc(100% - 28px);
      padding: 12px 14px;
      border-radius: 16px;
      background: rgba(2, 9, 13, 0.6);
      border: 1px solid rgba(255, 255, 255, 0.08);
      color: var(--muted);
      font-size: 13px;
      line-height: 1.45;
    }

    .sidebar {
      display: grid;
      gap: 18px;
      align-content: start;
    }

    .card h2 {
      margin: 0 0 6px;
      font-size: 20px;
      font-family: "Bahnschrift", "Trebuchet MS", sans-serif;
      letter-spacing: 0.02em;
    }

    .subtle {
      margin: 0 0 16px;
      color: var(--muted);
      font-size: 13px;
      line-height: 1.5;
    }

    .toolbar {
      align-items: center;
    }

    button,
    .button-link,
    select,
    input[type="text"] {
      font: inherit;
    }

    button,
    .button-link {
      display: inline-flex;
      align-items: center;
      justify-content: center;
      gap: 8px;
      min-height: 44px;
      padding: 0 16px;
      border-radius: 14px;
      border: 0;
      text-decoration: none;
      cursor: pointer;
      color: #03131a;
      background: linear-gradient(135deg, var(--accent) 0%, #7df0d8 100%);
      font-weight: 700;
      transition: transform 0.18s ease, opacity 0.18s ease, box-shadow 0.18s ease;
      box-shadow: 0 12px 30px rgba(65, 212, 183, 0.22);
    }

    button.secondary,
    .button-link.secondary {
      background: rgba(255, 255, 255, 0.06);
      color: var(--text);
      border: 1px solid rgba(255, 255, 255, 0.08);
      box-shadow: none;
    }

    button.warn {
      background: linear-gradient(135deg, var(--accent-2) 0%, #ffd08e 100%);
      color: #241100;
      box-shadow: 0 12px 30px rgba(255, 179, 92, 0.2);
    }

    button.danger {
      background: linear-gradient(135deg, var(--danger) 0%, #ff9f9f 100%);
      color: #250405;
      box-shadow: 0 12px 30px rgba(255, 113, 113, 0.22);
    }

    button:hover,
    .button-link:hover {
      transform: translateY(-1px);
    }

    button:disabled {
      opacity: 0.45;
      cursor: wait;
      transform: none;
    }

    .slider-grid,
    .toggle-grid {
      display: grid;
      grid-template-columns: repeat(2, minmax(0, 1fr));
      gap: 12px;
    }

    .slider,
    .toggle,
    .field,
    .stat {
      padding: 14px;
      border-radius: 16px;
      border: 1px solid rgba(255, 255, 255, 0.08);
      background: rgba(255, 255, 255, 0.04);
    }

    .slider header,
    .field label,
    .toggle label,
    .stat strong {
      display: flex;
      justify-content: space-between;
      gap: 12px;
      align-items: center;
      margin-bottom: 10px;
      font-size: 14px;
    }

    .slider small,
    .toggle small,
    .field small,
    .stat span {
      color: var(--muted);
      line-height: 1.45;
    }

    input[type="range"] {
      width: 100%;
      accent-color: var(--accent);
    }

    select,
    input[type="text"] {
      width: 100%;
      min-height: 42px;
      padding: 10px 12px;
      border-radius: 12px;
      border: 1px solid rgba(255, 255, 255, 0.08);
      background: rgba(4, 17, 24, 0.82);
      color: var(--text);
      outline: none;
    }

    .toggle {
      display: grid;
      gap: 10px;
      align-content: start;
    }

    .toggle-top {
      display: flex;
      justify-content: space-between;
      gap: 12px;
      align-items: flex-start;
    }

    .switch {
      position: relative;
      display: inline-flex;
      width: 54px;
      height: 32px;
      flex: 0 0 auto;
    }

    .switch input {
      opacity: 0;
      width: 0;
      height: 0;
    }

    .switch span {
      position: absolute;
      inset: 0;
      border-radius: 999px;
      background: rgba(255, 255, 255, 0.15);
      border: 1px solid rgba(255, 255, 255, 0.08);
      transition: 0.2s ease;
    }

    .switch span::after {
      content: "";
      position: absolute;
      top: 3px;
      left: 3px;
      width: 24px;
      height: 24px;
      border-radius: 50%;
      background: #f1fcff;
      transition: 0.2s ease;
      box-shadow: 0 6px 18px rgba(0, 0, 0, 0.25);
    }

    .switch input:checked + span {
      background: rgba(65, 212, 183, 0.24);
      border-color: rgba(65, 212, 183, 0.45);
    }

    .switch input:checked + span::after {
      transform: translateX(22px);
      background: #7cf0d8;
    }

    .face-controls {
      display: grid;
      gap: 10px;
    }

    .face-list {
      min-height: 42px;
    }

    .face-pill {
      padding: 8px 10px;
      border-radius: 999px;
      background: rgba(255, 255, 255, 0.07);
      border: 1px solid rgba(255, 255, 255, 0.08);
      font-size: 13px;
      color: #eff9ff;
    }

    .stat-grid {
      display: grid;
      grid-template-columns: repeat(2, minmax(0, 1fr));
      gap: 12px;
    }

    .stat strong {
      margin-bottom: 6px;
      font-size: 12px;
      letter-spacing: 0.05em;
      text-transform: uppercase;
      color: var(--muted);
    }

    .stat .value {
      font-size: 22px;
      font-family: "Bahnschrift", "Trebuchet MS", sans-serif;
    }

    .footer-note {
      color: var(--muted);
      font-size: 12px;
      line-height: 1.5;
    }

    @media (max-width: 1100px) {
      .content {
        grid-template-columns: 1fr;
      }
    }

    @media (max-width: 740px) {
      .layout {
        width: min(100% - 16px, 1500px);
        margin-top: 12px;
      }

      .hero,
      .card,
      .stage-card {
        padding: 14px;
        border-radius: 18px;
      }

      .slider-grid,
      .toggle-grid,
      .stat-grid {
        grid-template-columns: 1fr;
      }

      .toolbar {
        flex-direction: column;
        align-items: stretch;
      }

      button,
      .button-link {
        width: 100%;
      }
    }
  </style>
</head>
<body>
  <main class="layout">
    <section class="hero">
      <div class="hero-top">
        <div>
          <div class="eyebrow">ESP32-CAM AI Thinker • Vision Deck</div>
          <h1>Камера, стрим и AI-оверлей в одной панели.</h1>
          <p class="hero-copy">
            Поток идёт прямо с ESP32-CAM, а AI-режимы для лица, человека и рук работают в браузере поверх потока.
            Если Wi-Fi проседает, адаптивный режим сам понижает разрешение и лимит FPS, но держит картинку читаемой.
          </p>
        </div>
        <div class="hero-meta">
          <div class="chip"><strong id="heroIp">--</strong><span>IP камеры</span></div>
          <div class="chip"><strong id="heroRssi">--</strong><span>Wi-Fi</span></div>
          <div class="chip"><strong id="heroMode">Adaptive</strong><span>Профиль</span></div>
        </div>
      </div>
      <div class="chip-row">
        <div class="chip"><strong id="activeResolution">--</strong><span>Текущее разрешение</span></div>
        <div class="chip"><strong id="activeFps">--</strong><span>Лимит потока</span></div>
        <div class="chip"><strong id="activeQuality">--</strong><span>JPEG quality</span></div>
        <div class="chip"><strong id="streamState">Initializing</strong><span>Статус потока</span></div>
      </div>
    </section>

    <section class="content">
      <section class="stage-card hero">
        <div class="stage-wrap">
          <img id="stream" alt="ESP32 stream" crossorigin="anonymous">
          <canvas id="overlay"></canvas>
          <div class="stage-hud">
            <div class="stage-badge" id="aiBadge">AI: idle</div>
            <div class="stage-badge" id="networkBadge">Wi-Fi: --</div>
          </div>
          <div class="stage-note" id="stageNote">
            Поток MJPEG идёт с порта <strong>:81</strong>. AI-режимам нужен интернет на устройстве клиента только для первой загрузки моделей.
          </div>
        </div>

        <div class="toolbar">
          <button id="streamBtn">Перезапустить поток</button>
          <button id="snapshotBtn" class="warn">Сделать снимок</button>
          <a id="snapshotLink" class="button-link secondary" href="#" download="esp32-cam.jpg">Скачать последний кадр</a>
          <button id="refreshBtn" class="secondary">Обновить статус</button>
        </div>
      </section>

      <aside class="sidebar">
        <section class="card">
          <h2>Поток и изображение</h2>
          <p class="subtle">Ручные параметры задают желаемый максимум. При включённой адаптации камера может временно опускаться ниже.</p>

          <div class="field">
            <label for="framesize">Разрешение</label>
            <select id="framesize"></select>
            <small>Рекомендуемый баланс: <strong>HD 1280x720</strong> или <strong>VGA 640x480</strong>.</small>
          </div>

          <div class="slider-grid">
            <div class="slider">
              <header><span>JPEG quality</span><strong id="qualityValue">10</strong></header>
              <input id="quality" type="range" min="10" max="63" step="1" value="10">
              <small>Меньше значение = лучше качество и больше нагрузка на канал.</small>
            </div>

            <div class="slider">
              <header><span>Лимит FPS</span><strong id="fpsValue">14</strong></header>
              <input id="streamFps" type="range" min="4" max="20" step="1" value="14">
              <small>Это программный лимит для MJPEG-потока, не прямой сенсорный FPS.</small>
            </div>

            <div class="slider">
              <header><span>Фонарик</span><strong id="flashValue">0</strong></header>
              <input id="flash" type="range" min="0" max="255" step="1" value="0">
              <small>Подсветка на GPIO4. Для постоянного света лучше не ставить максимум надолго.</small>
            </div>

            <div class="slider">
              <header><span>Яркость</span><strong id="brightnessValue">1</strong></header>
              <input id="brightness" type="range" min="-2" max="2" step="1" value="1">
              <small>Полезно, если картинка чуть темнее, чем хочется.</small>
            </div>

            <div class="slider">
              <header><span>Контраст</span><strong id="contrastValue">1</strong></header>
              <input id="contrast" type="range" min="-2" max="2" step="1" value="1">
              <small>Добавляет выразительности объектам при обычном дневном свете.</small>
            </div>

            <div class="slider">
              <header><span>Насыщенность</span><strong id="saturationValue">0</strong></header>
              <input id="saturation" type="range" min="-2" max="2" step="1" value="0">
              <small>Небольшое изменение помогает при тёплом комнатном освещении.</small>
            </div>
          </div>

          <div class="toggle-grid" style="margin-top:12px;">
            <div class="toggle">
              <div class="toggle-top">
                <div>
                  <label for="adaptiveToggle">Адаптация Wi-Fi</label>
                  <small>Смотрит на RSSI и в плохом канале снижает разрешение и FPS.</small>
                </div>
                <label class="switch">
                  <input id="adaptiveToggle" type="checkbox" checked>
                  <span></span>
                </label>
              </div>
            </div>

            <div class="toggle">
              <div class="toggle-top">
                <div>
                  <label for="hmirrorToggle">Зеркало по горизонтали</label>
                  <small>Удобно, если нужен selfie-вид.</small>
                </div>
                <label class="switch">
                  <input id="hmirrorToggle" type="checkbox">
                  <span></span>
                </label>
              </div>
            </div>

            <div class="toggle">
              <div class="toggle-top">
                <div>
                  <label for="vflipToggle">Перевернуть вертикально</label>
                  <small>Используй, если модуль закреплён вверх ногами.</small>
                </div>
                <label class="switch">
                  <input id="vflipToggle" type="checkbox">
                  <span></span>
                </label>
              </div>
            </div>
          </div>
        </section>

        <section class="card">
          <h2>AI в браузере</h2>
          <p class="subtle">Определение лица, человека и рук работает поверх потока в браузере. Сам стрим идёт и без интернета, но для AI-моделей клиенту нужен хотя бы первый онлайн-запуск.</p>

          <div class="toggle-grid">
            <div class="toggle">
              <div class="toggle-top">
                <div>
                  <label for="faceToggle">Детект лица</label>
                  <small>Показывает прямоугольник вокруг лица.</small>
                </div>
                <label class="switch">
                  <input id="faceToggle" type="checkbox">
                  <span></span>
                </label>
              </div>
            </div>

            <div class="toggle">
              <div class="toggle-top">
                <div>
                  <label for="personToggle">Детект человека</label>
                  <small>Ищет человека на кадре моделью object detection.</small>
                </div>
                <label class="switch">
                  <input id="personToggle" type="checkbox">
                  <span></span>
                </label>
              </div>
            </div>

            <div class="toggle">
              <div class="toggle-top">
                <div>
                  <label for="handsToggle">Детект рук</label>
                  <small>Рисует рамку и ключевые точки на кисти.</small>
                </div>
                <label class="switch">
                  <input id="handsToggle" type="checkbox">
                  <span></span>
                </label>
              </div>
            </div>

            <div class="toggle">
              <div class="toggle-top">
                <div>
                  <label for="recognitionToggle">Распознавание лица</label>
                  <small>Подписывает сохранённые лица. Нужна кнопка «Запомнить лицо» ниже.</small>
                </div>
                <label class="switch">
                  <input id="recognitionToggle" type="checkbox">
                  <span></span>
                </label>
              </div>
            </div>
          </div>

          <div class="face-controls" style="margin-top:12px;">
            <div class="field" style="margin:0;">
              <label for="faceLabel">Имя для лица</label>
              <input id="faceLabel" type="text" value="Я" placeholder="Например: Я">
              <small>Смотри в камеру, затем нажми кнопку ниже, и браузер сохранит descriptor в localStorage.</small>
            </div>
            <div class="toolbar">
              <button id="rememberFaceBtn" class="secondary">Запомнить лицо</button>
              <button id="clearFacesBtn" class="danger">Очистить лица</button>
            </div>
            <div class="face-list" id="faceList"></div>
          </div>
        </section>

        <section class="card">
          <h2>Телеметрия</h2>
          <p class="subtle">Здесь видно, как камера реально работает сейчас, а не только что выбрано вручную.</p>

          <div class="stat-grid">
            <div class="stat">
              <strong>Wi-Fi RSSI</strong>
              <div class="value" id="statRssi">--</div>
              <span id="statRssiNote">Ожидание статуса...</span>
            </div>
            <div class="stat">
              <strong>SSID</strong>
              <div class="value" id="statSsid" style="font-size:18px;">--</div>
              <span>Сеть, в которую сейчас вошла камера.</span>
            </div>
            <div class="stat">
              <strong>Применённый профиль</strong>
              <div class="value" id="statApplied">--</div>
              <span id="statAppliedNote">Разрешение / FPS / quality</span>
            </div>
            <div class="stat">
              <strong>AI статус</strong>
              <div class="value" id="aiStatus">Idle</div>
              <span id="aiStatusNote">Модели не загружены.</span>
            </div>
          </div>

          <p class="footer-note" style="margin-top:14px;">
            Совет: для удалённого доступа из любой точки мира лучше не открывать ESP32 напрямую в интернет.
            Гораздо безопаснее зайти через Tailscale, WireGuard или Home Assistant / NVR в локальной сети.
          </p>
        </section>
      </aside>
    </section>
  </main>

  <script>
    const FRAME_OPTIONS = [
      { value: 15, label: "UXGA 1600x1200" },
      { value: 13, label: "HD 1280x720" },
      { value: 12, label: "XGA 1024x768" },
      { value: 11, label: "SVGA 800x600" },
      { value: 10, label: "VGA 640x480" },
      { value: 6, label: "QVGA 320x240" }
    ];

    const FACE_MODEL_URL = "https://justadudewhohacks.github.io/face-api.js/models";
    const workingCanvas = document.createElement("canvas");
    const workingCtx = workingCanvas.getContext("2d", { willReadFrequently: true });

    const state = {
      streamReady: false,
      aiBusy: false,
      aiDelay: 850,
      faceMatcher: null,
      models: {
        tfReady: false,
        faceReady: false,
        person: null,
        hands: null
      },
      ai: {
        face: false,
        person: false,
        hands: false,
        recognition: false
      },
      knownFaces: loadKnownFaces()
    };

    const els = {};

    document.addEventListener("DOMContentLoaded", init);

    function init() {
      [
        "stream", "overlay", "framesize", "quality", "streamFps", "flash", "brightness", "contrast", "saturation",
        "qualityValue", "fpsValue", "flashValue", "brightnessValue", "contrastValue", "saturationValue",
        "adaptiveToggle", "hmirrorToggle", "vflipToggle", "streamBtn", "snapshotBtn", "snapshotLink", "refreshBtn",
        "faceToggle", "personToggle", "handsToggle", "recognitionToggle", "rememberFaceBtn", "clearFacesBtn",
        "faceLabel", "faceList", "heroIp", "heroRssi", "heroMode", "activeResolution", "activeFps", "activeQuality",
        "streamState", "aiBadge", "networkBadge", "stageNote", "statRssi", "statRssiNote", "statSsid", "statApplied",
        "statAppliedNote", "aiStatus", "aiStatusNote"
      ].forEach((id) => {
        els[id] = document.getElementById(id);
      });

      populateFrameOptions();
      bindControls();
      rebuildFaceMatcher();
      renderKnownFaces();
      restartStream();
      refreshStatus();
      setInterval(refreshStatus, 5000);
      window.addEventListener("resize", syncOverlay);
      setTimeout(aiLoop, 1400);
    }

    function populateFrameOptions() {
      els.framesize.innerHTML = FRAME_OPTIONS.map((item) => `<option value="${item.value}">${item.label}</option>`).join("");
    }

    function bindControls() {
      els.streamBtn.addEventListener("click", restartStream);
      els.refreshBtn.addEventListener("click", () => refreshStatus(true));
      els.snapshotBtn.addEventListener("click", captureSnapshot);
      els.rememberFaceBtn.addEventListener("click", rememberFace);
      els.clearFacesBtn.addEventListener("click", clearFaces);

      bindSlider(els.quality, els.qualityValue, async (value) => sendControl("quality", value));
      bindSlider(els.streamFps, els.fpsValue, async (value) => sendControl("stream_fps", value));
      bindSlider(els.flash, els.flashValue, debounce(async (value) => sendControl("led_intensity", value), 120));
      bindSlider(els.brightness, els.brightnessValue, async (value) => sendControl("brightness", value));
      bindSlider(els.contrast, els.contrastValue, async (value) => sendControl("contrast", value));
      bindSlider(els.saturation, els.saturationValue, async (value) => sendControl("saturation", value));

      els.framesize.addEventListener("change", async () => sendControl("framesize", els.framesize.value));
      els.adaptiveToggle.addEventListener("change", async () => sendControl("adaptive", els.adaptiveToggle.checked ? 1 : 0));
      els.hmirrorToggle.addEventListener("change", async () => sendControl("hmirror", els.hmirrorToggle.checked ? 1 : 0));
      els.vflipToggle.addEventListener("change", async () => sendControl("vflip", els.vflipToggle.checked ? 1 : 0));

      [
        ["face", els.faceToggle],
        ["person", els.personToggle],
        ["hands", els.handsToggle],
        ["recognition", els.recognitionToggle]
      ].forEach(([key, checkbox]) => {
        checkbox.addEventListener("change", async () => {
          state.ai[key] = checkbox.checked;
          try {
            if (checkbox.checked) {
              await ensureModelsForToggle(key);
            }
            setAiStatus(summaryAiState(), "AI-режим переключён.");
          } catch (error) {
            checkbox.checked = false;
            state.ai[key] = false;
            setAiStatus("AI error", error.message || "Не удалось загрузить модель.");
          }
        });
      });

      els.stream.addEventListener("load", () => {
        state.streamReady = true;
        syncOverlay();
        els.streamState.textContent = "Live";
      });

      els.stream.addEventListener("error", () => {
        state.streamReady = false;
        els.streamState.textContent = "Reconnect";
      });
    }

    function bindSlider(input, label, commit) {
      const updateLabel = () => {
        label.textContent = input.value;
      };

      updateLabel();
      input.addEventListener("input", updateLabel);
      input.addEventListener("change", async () => commit(input.value));
    }

    async function refreshStatus(isManual = false) {
      try {
        const response = await fetch("/status", { cache: "no-store" });
        if (!response.ok) {
          throw new Error(`HTTP ${response.status}`);
        }
        const status = await response.json();
        applyStatus(status);
        if (isManual) {
          setAiStatus(summaryAiState(), "Статус обновлён с камеры.");
        }
      } catch (error) {
        setAiStatus("Status error", "Не удалось прочитать /status.");
        els.streamState.textContent = "Status fail";
      }
    }

    function applyStatus(status) {
      window.lastStatusData = status;
      const frameLabel = frameLabelByValue(status.applied_framesize ?? status.framesize);
      const requestedFrameLabel = frameLabelByValue(status.manual_framesize ?? status.framesize);
      const rssiText = status.ap_mode ? `AP • ${status.ap_clients || 0} client(s)` : (Number.isFinite(status.wifi_rssi) ? `${status.wifi_rssi} dBm` : "--");
      const profileText = `${frameLabel} / ${status.applied_stream_fps} fps / q${status.applied_quality}`;

      els.framesize.value = String(status.manual_framesize ?? status.framesize ?? 13);
      setRangeValue(els.quality, els.qualityValue, status.manual_quality ?? status.quality);
      setRangeValue(els.streamFps, els.fpsValue, status.stream_fps ?? 14);
      setRangeValue(els.flash, els.flashValue, status.led_intensity ?? 0);
      setRangeValue(els.brightness, els.brightnessValue, status.brightness ?? 1);
      setRangeValue(els.contrast, els.contrastValue, status.contrast ?? 1);
      setRangeValue(els.saturation, els.saturationValue, status.saturation ?? 0);

      els.adaptiveToggle.checked = Boolean(status.adaptive);
      els.hmirrorToggle.checked = Boolean(status.hmirror);
      els.vflipToggle.checked = Boolean(status.vflip);

      els.heroIp.textContent = status.ip || location.hostname;
      els.heroRssi.textContent = rssiText;
      els.heroMode.textContent = status.ap_mode ? "Access Point" : (status.adaptive ? "Adaptive" : "Manual");
      els.activeResolution.textContent = frameLabel;
      els.activeFps.textContent = `${status.applied_stream_fps || 0} fps`;
      els.activeQuality.textContent = `q${status.applied_quality || status.quality || 0}`;
      els.networkBadge.textContent = status.ap_mode ? `AP: ${status.ssid || "Camera"}` : `Wi-Fi: ${rssiText}`;

      els.statRssi.textContent = rssiText;
      els.statRssiNote.textContent = rssiNote(status.wifi_rssi);
      els.statSsid.textContent = status.ssid || "--";
      els.statApplied.textContent = status.ap_mode ? "Access Point" : (status.adaptive ? "Adaptive" : "Manual");
      els.statAppliedNote.textContent = `${profileText} • requested ${requestedFrameLabel}`;
      els.stageNote.innerHTML = `Подключись к сети <strong>${status.ssid || "Camera"}</strong> и открой <strong>http://${status.ip || location.hostname}</strong>. Поток: <strong>${status.ip || location.hostname}:81/stream</strong>. Сейчас применён профиль <strong>${profileText}</strong>.`;
    }

    function setRangeValue(input, label, value) {
      if (value === undefined || value === null) {
        return;
      }
      input.value = value;
      label.textContent = value;
    }

    async function sendControl(name, value) {
      try {
        const response = await fetch(`/control?var=${encodeURIComponent(name)}&val=${encodeURIComponent(value)}`);
        if (!response.ok) {
          throw new Error(`HTTP ${response.status}`);
        }
        await refreshStatus();
      } catch (error) {
        setAiStatus("Control error", `Не удалось применить ${name}.`);
      }
    }

    function streamUrl() {
      return `${location.protocol}//${location.hostname}:81/stream?_cb=${Date.now()}`;
    }

    function captureUrl() {
      return `${location.origin}/capture?_cb=${Date.now()}`;
    }

    function restartStream() {
      els.streamState.textContent = "Restarting";
      state.streamReady = false;
      els.stream.src = "";
      setTimeout(() => {
        els.stream.src = streamUrl();
      }, 160);
    }

    async function captureSnapshot() {
      const href = captureUrl();
      els.snapshotLink.href = href;
      els.snapshotLink.click();
    }

    function syncOverlay() {
      const rect = els.stream.getBoundingClientRect();
      const ratio = window.devicePixelRatio || 1;
      els.overlay.width = Math.max(1, Math.round(rect.width * ratio));
      els.overlay.height = Math.max(1, Math.round(rect.height * ratio));
      els.overlay.style.width = `${rect.width}px`;
      els.overlay.style.height = `${rect.height}px`;
      const ctx = els.overlay.getContext("2d");
      ctx.setTransform(ratio, 0, 0, ratio, 0, 0);
      ctx.clearRect(0, 0, rect.width, rect.height);
    }

    function frameLabelByValue(value) {
      const match = FRAME_OPTIONS.find((item) => Number(item.value) === Number(value));
      return match ? match.label : `Frame ${value}`;
    }

    function rssiNote(rssi) {
      if (typeof window.lastStatusData === "object" && window.lastStatusData?.ap_mode) {
        return `Камера работает как точка доступа. Подключённых клиентов: ${window.lastStatusData.ap_clients || 0}.`;
      }
      if (!Number.isFinite(rssi)) {
        return "Сигнал ещё не получен.";
      }
      if (rssi >= -60) {
        return "Сигнал хороший. Камера может спокойно держать HD.";
      }
      if (rssi >= -70) {
        return "Сигнал средний. Адаптация может перейти на SVGA/VGA.";
      }
      return "Сигнал слабый. Держи адаптацию включённой.";
    }

    function anyAiEnabled() {
      return state.ai.face || state.ai.person || state.ai.hands || state.ai.recognition;
    }

    async function aiLoop() {
      if (!state.streamReady || !anyAiEnabled()) {
        clearOverlay();
        setTimeout(aiLoop, 900);
        return;
      }

      if (state.aiBusy) {
        setTimeout(aiLoop, 250);
        return;
      }

      state.aiBusy = true;
      try {
        const frame = copyStreamToWorkingCanvas();
        if (!frame) {
          throw new Error("Поток ещё не готов.");
        }

        const overlays = [];
        if (state.ai.face || state.ai.recognition) {
          overlays.push(...await detectFaces());
        }
        if (state.ai.person) {
          overlays.push(...await detectPeople());
        }
        if (state.ai.hands) {
          overlays.push(...await detectHands());
        }

        drawOverlay(overlays, frame.width, frame.height);
        els.aiBadge.textContent = overlays.length ? `AI: ${overlays.length} object(s)` : "AI: no hits";
        setAiStatus(summaryAiState(), overlays.length ? "Объекты найдены на текущем кадре." : "AI работает, но объектов сейчас нет.");
      } catch (error) {
        clearOverlay();
        setAiStatus("AI paused", error.message || "AI временно недоступен.");
      } finally {
        state.aiBusy = false;
        setTimeout(aiLoop, state.ai.recognition ? 1200 : state.aiDelay);
      }
    }

    function copyStreamToWorkingCanvas() {
      const width = els.stream.naturalWidth || 0;
      const height = els.stream.naturalHeight || 0;
      if (!width || !height) {
        return null;
      }
      workingCanvas.width = width;
      workingCanvas.height = height;
      workingCtx.drawImage(els.stream, 0, 0, width, height);
      return { width, height };
    }

    async function ensureModelsForToggle(toggle) {
      if (toggle === "face" || toggle === "recognition") {
        await ensureFaceModels();
        return;
      }
      if (toggle === "person") {
        await ensurePersonModel();
        return;
      }
      if (toggle === "hands") {
        await ensureHandsModel();
      }
    }

    async function ensureTfReady() {
      if (state.models.tfReady) {
        return;
      }
      if (!window.tf) {
        throw new Error("TensorFlow.js не загрузился. Нужен интернет.");
      }
      try {
        await tf.setBackend("webgl");
      } catch (error) {
        await tf.setBackend("cpu");
      }
      await tf.ready();
      state.models.tfReady = true;
    }

    async function ensureFaceModels() {
      if (state.models.faceReady) {
        return;
      }
      if (!window.faceapi) {
        throw new Error("Face API не загрузился. Проверь интернет.");
      }
      await ensureTfReady();
      setAiStatus("Loading face AI", "Загружаю модели распознавания лица...");
      await Promise.all([
        faceapi.nets.tinyFaceDetector.loadFromUri(FACE_MODEL_URL),
        faceapi.nets.faceLandmark68TinyNet.loadFromUri(FACE_MODEL_URL),
        faceapi.nets.faceRecognitionNet.loadFromUri(FACE_MODEL_URL)
      ]);
      state.models.faceReady = true;
      rebuildFaceMatcher();
    }

    async function ensurePersonModel() {
      if (state.models.person) {
        return;
      }
      if (!window.cocoSsd) {
        throw new Error("COCO-SSD не загрузился. Проверь интернет.");
      }
      await ensureTfReady();
      setAiStatus("Loading person AI", "Загружаю модель детекта человека...");
      state.models.person = await cocoSsd.load({ base: "lite_mobilenet_v2" });
    }

    async function ensureHandsModel() {
      if (state.models.hands) {
        return;
      }
      if (!window.handPoseDetection) {
        throw new Error("Hand pose model не загрузился. Проверь интернет.");
      }
      await ensureTfReady();
      setAiStatus("Loading hand AI", "Загружаю модель рук...");
      state.models.hands = await handPoseDetection.createDetector(
        handPoseDetection.SupportedModels.MediaPipeHands,
        { runtime: "tfjs", modelType: "lite", maxHands: 2 }
      );
    }

    async function detectFaces() {
      await ensureFaceModels();
      const options = new faceapi.TinyFaceDetectorOptions({ inputSize: 224, scoreThreshold: 0.45 });

      if (state.ai.recognition && state.faceMatcher) {
        const faces = await faceapi.detectAllFaces(workingCanvas, options).withFaceLandmarks(true).withFaceDescriptors();
        return faces.map((face) => {
          const match = state.faceMatcher.findBestMatch(face.descriptor);
          return {
            kind: "face",
            label: match.label === "unknown" ? "Лицо" : match.label,
            score: match.distance,
            box: face.detection.box
          };
        });
      }

      const faces = await faceapi.detectAllFaces(workingCanvas, options);
      return faces.map((face) => ({
        kind: "face",
        label: "Лицо",
        box: face.box || face.detection.box
      }));
    }

    async function detectPeople() {
      await ensurePersonModel();
      const predictions = await state.models.person.detect(workingCanvas, 6, 0.45);
      return predictions
        .filter((prediction) => prediction.class === "person" && prediction.score >= 0.45)
        .map((prediction) => ({
          kind: "person",
          label: `Человек ${(prediction.score * 100).toFixed(0)}%`,
          box: {
            x: prediction.bbox[0],
            y: prediction.bbox[1],
            width: prediction.bbox[2],
            height: prediction.bbox[3]
          }
        }));
    }

    async function detectHands() {
      await ensureHandsModel();
      const hands = await state.models.hands.estimateHands(workingCanvas, { flipHorizontal: false });
      return hands.map((hand, index) => {
        const points = (hand.keypoints || []).filter(Boolean);
        const xs = points.map((point) => point.x);
        const ys = points.map((point) => point.y);
        return {
          kind: "hand",
          label: `Рука ${index + 1}`,
          points,
          box: {
            x: Math.min(...xs),
            y: Math.min(...ys),
            width: Math.max(...xs) - Math.min(...xs),
            height: Math.max(...ys) - Math.min(...ys)
          }
        };
      });
    }

    function drawOverlay(objects, sourceWidth, sourceHeight) {
      syncOverlay();
      const canvas = els.overlay;
      const ctx = canvas.getContext("2d");
      const displayWidth = canvas.clientWidth;
      const displayHeight = canvas.clientHeight;
      const scaleX = displayWidth / sourceWidth;
      const scaleY = displayHeight / sourceHeight;

      ctx.clearRect(0, 0, displayWidth, displayHeight);
      ctx.lineWidth = 2;
      ctx.font = '600 14px "Bahnschrift", "Trebuchet MS", sans-serif';
      ctx.textBaseline = "top";

      objects.forEach((item) => {
        const color = item.kind === "person" ? "#ffb35c" : item.kind === "hand" ? "#7cb9ff" : "#41d4b7";
        const x = item.box.x * scaleX;
        const y = item.box.y * scaleY;
        const width = item.box.width * scaleX;
        const height = item.box.height * scaleY;

        ctx.strokeStyle = color;
        ctx.fillStyle = color;
        ctx.shadowColor = "rgba(0,0,0,0.32)";
        ctx.shadowBlur = 12;
        ctx.strokeRect(x, y, width, height);

        const label = item.label || item.kind;
        const textWidth = ctx.measureText(label).width + 16;
        ctx.fillRect(x, Math.max(8, y - 28), textWidth, 22);
        ctx.fillStyle = "#051018";
        ctx.fillText(label, x + 8, Math.max(11, y - 25));
        ctx.fillStyle = color;

        if (item.points && item.points.length) {
          item.points.forEach((point) => {
            ctx.beginPath();
            ctx.arc(point.x * scaleX, point.y * scaleY, 3, 0, Math.PI * 2);
            ctx.fill();
          });
        }
      });
    }

    function clearOverlay() {
      const ctx = els.overlay.getContext("2d");
      ctx.clearRect(0, 0, els.overlay.clientWidth, els.overlay.clientHeight);
      els.aiBadge.textContent = "AI: idle";
    }

    async function rememberFace() {
      const label = (els.faceLabel.value || "").trim() || "Я";
      try {
        await ensureFaceModels();
        const frame = copyStreamToWorkingCanvas();
        if (!frame) {
          throw new Error("Нет кадра для анализа.");
        }

        const detection = await faceapi
          .detectSingleFace(workingCanvas, new faceapi.TinyFaceDetectorOptions({ inputSize: 224, scoreThreshold: 0.45 }))
          .withFaceLandmarks(true)
          .withFaceDescriptor();

        if (!detection) {
          throw new Error("Лицо не найдено. Посмотри прямо в камеру и попробуй ещё раз.");
        }

        const descriptor = Array.from(detection.descriptor);
        const existing = state.knownFaces.findIndex((item) => item.label === label);
        if (existing >= 0) {
          state.knownFaces[existing] = { label, descriptor };
        } else {
          state.knownFaces.push({ label, descriptor });
        }
        saveKnownFaces(state.knownFaces);
        rebuildFaceMatcher();
        renderKnownFaces();
        setAiStatus("Face saved", `Лицо «${label}» сохранено локально в браузере.`);
      } catch (error) {
        setAiStatus("Face save failed", error.message || "Не удалось запомнить лицо.");
      }
    }

    function clearFaces() {
      state.knownFaces = [];
      saveKnownFaces(state.knownFaces);
      rebuildFaceMatcher();
      renderKnownFaces();
      setAiStatus("Faces cleared", "Сохранённые лица удалены.");
    }

    function rebuildFaceMatcher() {
      if (!window.faceapi || !state.knownFaces.length) {
        state.faceMatcher = null;
        return;
      }
      const descriptors = state.knownFaces.map((entry) => new faceapi.LabeledFaceDescriptors(
        entry.label,
        [new Float32Array(entry.descriptor)]
      ));
      state.faceMatcher = new faceapi.FaceMatcher(descriptors, 0.55);
    }

    function renderKnownFaces() {
      if (!state.knownFaces.length) {
        els.faceList.innerHTML = '<div class="face-pill">Пока ничего не сохранено</div>';
        return;
      }
      els.faceList.innerHTML = state.knownFaces
        .map((entry) => `<div class="face-pill">${escapeHtml(entry.label)}</div>`)
        .join("");
    }

    function loadKnownFaces() {
      try {
        return JSON.parse(localStorage.getItem("esp32-cam-known-faces") || "[]");
      } catch (error) {
        return [];
      }
    }

    function saveKnownFaces(faces) {
      localStorage.setItem("esp32-cam-known-faces", JSON.stringify(faces));
    }

    function escapeHtml(text) {
      return text.replace(/[&<>"]/g, (symbol) => ({ "&": "&amp;", "<": "&lt;", ">": "&gt;", '"': "&quot;" }[symbol]));
    }

    function setAiStatus(title, note) {
      els.aiStatus.textContent = title;
      els.aiStatusNote.textContent = note;
    }

    function summaryAiState() {
      const active = [];
      if (state.ai.face) active.push("face");
      if (state.ai.person) active.push("person");
      if (state.ai.hands) active.push("hands");
      if (state.ai.recognition) active.push("recognition");
      return active.length ? `AI: ${active.join(", ")}` : "AI idle";
    }

    function debounce(callback, delay) {
      let timer = null;
      return (...args) => {
        clearTimeout(timer);
        timer = setTimeout(() => callback(...args), delay);
      };
    }
  </script>
</body>
</html>
)HTML";

#endif
