import csv
import math
import sys
from typing import List, Tuple

try:
    import matplotlib.pyplot as plt
except ImportError:
    plt = None


def load_ecg(path: str) -> Tuple[List[float], List[float]]:
    t: List[float] = []
    x: List[float] = []
    with open(path, newline="") as f:
        # Detect CSV header or plain space-separated
        first_line = f.readline()
        if "," in first_line:
            # CSV; rewind and parse DictReader or split
            f.seek(0)
            reader = csv.reader(f)
            header = next(reader)
            lower = [h.strip().lower() for h in header]
            if "value" in lower and "time" in lower:
                idx_val = lower.index("value")
                idx_time = lower.index("time")
            else:
                # Assume two columns: try (value,time)
                idx_val, idx_time = 0, 1
            for row in reader:
                if len(row) < 2:
                    continue
                try:
                    val = float(row[idx_val])
                    tim = float(row[idx_time])
                except ValueError:
                    # Try reversed
                    try:
                        tim = float(row[idx_val])
                        val = float(row[idx_time])
                    except Exception:
                        continue
                x.append(val)
                t.append(tim)
        else:
            # Space separated: consume first line and rest
            def parse_line(line: str):
                parts = line.strip().split()
                if len(parts) < 2:
                    return None
                try:
                    a = float(parts[0]); b = float(parts[1])
                    return a, b
                except ValueError:
                    try:
                        b = float(parts[0]); a = float(parts[1])
                        return a, b
                    except Exception:
                        return None
            parsed = parse_line(first_line)
            if parsed:
                x.append(parsed[0]); t.append(parsed[1])
            for line in f:
                p = parse_line(line)
                if not p:
                    continue
                x.append(p[0]); t.append(p[1])
    return t, x


def estimate_dt(t: List[float]) -> float:
    if len(t) < 2:
        return -1.0
    diffs = [t[i+1] - t[i] for i in range(len(t)-1) if t[i+1] > t[i]]
    return sum(diffs)/len(diffs) if diffs else -1.0


def lowpass_iir(x: List[float], cutoff_hz: float, dt: float) -> List[float]:
    if dt <= 0:
        dt = 1.0/250.0
    # alpha = exp(-2*pi*fc*dt)
    pi = math.acos(-1.0)
    alpha = math.exp(-2.0 * pi * cutoff_hz * dt)
    b = 1.0 - alpha
    y: List[float] = [0.0]*len(x)
    if not x:
        return y
    y[0] = x[0]
    for i in range(1, len(x)):
        y[i] = alpha*y[i-1] + b*x[i]
    return y


def detect_peaks(t: List[float], y: List[float], threshold: float, window: int) -> List[float]:
    peaks: List[float] = []
    if window < 1:
        window = 1
    n = len(y)
    i = 0
    while i < n:
        val = y[i]
        if val >= threshold:
            # check local maximum within window
            left_ok = all(y[j] < val for j in range(max(0, i-window), i))
            right_ok = all(y[j] <= val for j in range(i+1, min(n, i+1+window)))
            if left_ok and right_ok:
                peaks.append(t[i])
                i += window  # skip neighbors
                continue
        i += 1
    return peaks


def bpm_from_peaks(peaks: List[float]) -> float:
    if len(peaks) < 2:
        return 0.0
    rrs = [peaks[i] - peaks[i-1] for i in range(1, len(peaks)) if peaks[i] > peaks[i-1]]
    if not rrs:
        return 0.0
    rr_avg = sum(rrs)/len(rrs)
    return 60.0/rr_avg


def save_outputs(path_base: str, t: List[float], x: List[float], y: List[float]):
    # TXT space-separated
    txt_path = path_base + "_filtrada.txt"
    with open(txt_path, "w", newline="") as f:
        f.write("# amplitud_original filtrada tiempo\n")
        for xi, yi, ti in zip(x, y, t):
            f.write(f"{xi} {yi} {ti}\n")
    # CSV with header
    csv_path = path_base + "_filtrada.csv"
    with open(csv_path, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["amplitud","filtrada","tiempo"])
        for xi, yi, ti in zip(x, y, t):
            w.writerow([xi, yi, ti])
    return txt_path, csv_path


def main():
    default_path = r"C:\\Users\\danre\\Desktop\\Punto 2 - Proyecto\\SenalECG.txt"
    path = sys.argv[1] if len(sys.argv) > 1 else default_path
    t, x = load_ecg(path)
    if not t:
        print(f"No se pudo cargar: {path}")
        return 1
    dt = estimate_dt(t)
    y = lowpass_iir(x, cutoff_hz=15.0, dt=dt)
    # Auto threshold: 45% of filtered max
    thr = 0.45*max(y) if y else 0.0
    peaks = detect_peaks(t, y, threshold=thr, window=10)
    bpm = bpm_from_peaks(peaks)
    print(f"Muestras: {len(x)}")
    print(f"Tiempo: {t[0]}s a {t[-1]}s")
    print(f"Picos detectados: {len(peaks)}")
    print(f"Frecuencia cardiaca promedio: {bpm:.2f} BPM")
    # save outputs next to input name
    base = path.rsplit(".", 1)[0]
    txt, csvp = save_outputs(base, t, x, y)
    # save peaks times
    with open(base + "_picos.txt", "w", newline="") as f:
        for ti in peaks:
            f.write(f"{ti}\n")
    print(f"Escritos: {txt} y {csvp}")
    if plt:
        plt.figure(figsize=(10,4))
        plt.plot(t, x, label="ECG", color="blue", linewidth=0.8)
        plt.plot(t, y, label="ECG filtrada", color="red", linewidth=1.5)
        if peaks:
            yp = [y[t.index(tp)] if tp in t else None for tp in peaks]
            plt.scatter(peaks, [yy for yy in yp if yy is not None], color="green", s=20, label="Picos")
        plt.xlabel("Tiempo (s)")
        plt.ylabel("Amplitud")
        plt.legend(); plt.grid(True, alpha=0.3); plt.tight_layout(); plt.show()
    else:
        print("matplotlib no está instalado; para ver la gráfica: pip install matplotlib")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
