; ================================
;  Sand Plotter — Rectangle Pattern
;  Bed: 280 x 380 mm
;  Margin: 12 mm from each edge
; ================================

G21        ; millimeters
G90        ; absolute positioning
G92 X0 Y0  ; set current position as origin

; --- Rapid to start corner ---
G0 F4000 X12 Y12

; --- Draw rectangle ---
G1 F1200 X268 Y12    ; → bottom-right
G1       X268 Y368   ; ↑ top-right
G1       X12  Y368   ; ← top-left
G1       X12  Y12    ; ↓ back to start (close)

; --- Return home ---
G0 F4000 X0 Y0
M2  ; end
