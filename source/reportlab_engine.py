"""
reportlab_engine.py - Core styling, canvas, and layout engine for the 
Practical Time Machine 3-Volume Technical Documentation.
Ensures exact 40-page generation with high aesthetic quality and scientific rigor.
"""

import os
from reportlab.lib.pagesizes import A4
from reportlab.lib import colors
from reportlab.pdfgen import canvas
from reportlab.platypus import (
    SimpleDocTemplate, Paragraph, Spacer, Table, TableStyle, PageBreak, Image, KeepTogether, HRFlowable
)
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.lib.enums import TA_CENTER, TA_LEFT, TA_RIGHT, TA_JUSTIFY

# Color Palette: Dark Navy, White, Cyan Accents, Slate Borders
PRIMARY_NAVY = colors.HexColor('#0B192C')
SECONDARY_NAVY = colors.HexColor('#1E3E62')
ACCENT_CYAN = colors.HexColor('#008B8B')
HIGHLIGHT_BLUE = colors.HexColor('#007ACC')
LIGHT_BG = colors.HexColor('#F8FAFC')
ALT_ROW_BG = colors.HexColor('#F1F5F9')
BORDER_COLOR = colors.HexColor('#CBD5E1')
ALERT_RED = colors.HexColor('#C92A2A')
TEXT_DARK = colors.HexColor('#1E293B')
TEXT_MUTED = colors.HexColor('#64748B')

class NumberedCanvas(canvas.Canvas):
    """
    Two-pass canvas that adds running headers, running footers,
    and exact 'Page X of 40' numbering.
    """
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._saved_page_states = []

    def showPage(self):
        self._saved_page_states.append(dict(self.__dict__))
        self._startPage()

    def save(self):
        num_pages = len(self._saved_page_states)
        for state in self._saved_page_states:
            self.__dict__.update(state)
            self.draw_page_decorations(num_pages)
            super().showPage()
        super().save()

    def draw_page_decorations(self, total_pages):
        page_num = self._pageNumber
        
        # Suppress headers and footers on Cover Page (Page 1)
        if page_num == 1:
            return

        self.saveState()
        
        # Running Header (Top at y = 805)
        self.setStrokeColor(BORDER_COLOR)
        self.setLineWidth(0.75)
        self.line(40, 805, 555, 805)
        
        self.setFont("Helvetica-Bold", 7.5)
        self.setFillColor(SECONDARY_NAVY)
        self.drawString(40, 810, "PRACTICAL TIME MACHINE: ADVANCED SPACETIME ENGINEERING RESEARCH")
        
        self.setFont("Helvetica", 7.5)
        self.setFillColor(TEXT_MUTED)
        self.drawRightString(555, 810, getattr(self, 'doc_subtitle', 'TECHNICAL DOCUMENTATION'))

        # Running Footer (Bottom at y = 38)
        self.setStrokeColor(BORDER_COLOR)
        self.setLineWidth(0.75)
        self.line(40, 38, 555, 38)
        
        self.setFont("Helvetica", 7.5)
        self.setFillColor(TEXT_MUTED)
        self.drawString(40, 27, "UNCLASSIFIED / SCIENTIFIC RESEARCH SPECIFICATION — NASA-LEVEL TRACEABILITY")
        
        page_str = f"Page {page_num} of {total_pages}"
        self.setFont("Helvetica-Bold", 7.5)
        self.setFillColor(PRIMARY_NAVY)
        self.drawRightString(555, 27, page_str)
        
        self.restoreState()


def get_project_styles():
    """Generates custom typography styles for the technical documentation."""
    styles = getSampleStyleSheet()

    title_style = ParagraphStyle(
        'DocTitle',
        parent=styles['Heading1'],
        fontName='Helvetica-Bold',
        fontSize=20,
        leading=24,
        textColor=PRIMARY_NAVY,
        alignment=TA_LEFT,
        spaceAfter=6
    )

    subtitle_style = ParagraphStyle(
        'DocSubtitle',
        parent=styles['Normal'],
        fontName='Helvetica',
        fontSize=11,
        leading=15,
        textColor=SECONDARY_NAVY,
        alignment=TA_LEFT,
        spaceAfter=15
    )

    h1_style = ParagraphStyle(
        'SectionH1',
        parent=styles['Heading1'],
        fontName='Helvetica-Bold',
        fontSize=12,
        leading=15,
        textColor=PRIMARY_NAVY,
        spaceBefore=4,
        spaceAfter=4,
        keepWithNext=True
    )

    h2_style = ParagraphStyle(
        'SectionH2',
        parent=styles['Heading2'],
        fontName='Helvetica-Bold',
        fontSize=9.5,
        leading=12,
        textColor=SECONDARY_NAVY,
        spaceBefore=3,
        spaceAfter=3,
        keepWithNext=True
    )

    body_style = ParagraphStyle(
        'DocBody',
        parent=styles['Normal'],
        fontName='Helvetica',
        fontSize=8.0,
        leading=10.5,
        textColor=TEXT_DARK,
        alignment=TA_LEFT,
        spaceAfter=3
    )

    body_justify = ParagraphStyle(
        'DocBodyJustify',
        parent=body_style,
        alignment=TA_JUSTIFY
    )

    bullet_style = ParagraphStyle(
        'DocBullet',
        parent=body_style,
        leftIndent=12,
        firstLineIndent=-8,
        spaceAfter=2
    )

    callout_style = ParagraphStyle(
        'CalloutText',
        parent=styles['Normal'],
        fontName='Helvetica-Oblique',
        fontSize=7.8,
        leading=10.0,
        textColor=PRIMARY_NAVY
    )

    equation_style = ParagraphStyle(
        'EquationStyle',
        parent=styles['Normal'],
        fontName='Courier-Bold',
        fontSize=8.5,
        leading=11.5,
        textColor=PRIMARY_NAVY,
        alignment=TA_CENTER
    )

    table_header_style = ParagraphStyle(
        'TableHeader',
        parent=styles['Normal'],
        fontName='Helvetica-Bold',
        fontSize=7.5,
        leading=9.0,
        textColor=colors.white,
        alignment=TA_CENTER
    )

    table_cell_style = ParagraphStyle(
        'TableCell',
        parent=styles['Normal'],
        fontName='Helvetica',
        fontSize=7.2,
        leading=8.8,
        textColor=TEXT_DARK,
        alignment=TA_LEFT
    )

    table_cell_mono = ParagraphStyle(
        'TableCellMono',
        parent=styles['Normal'],
        fontName='Courier',
        fontSize=7.0,
        leading=8.5,
        textColor=TEXT_DARK,
        alignment=TA_LEFT
    )

    table_cell_center = ParagraphStyle(
        'TableCellCenter',
        parent=table_cell_style,
        alignment=TA_CENTER
    )

    caption_style = ParagraphStyle(
        'CaptionStyle',
        parent=styles['Normal'],
        fontName='Helvetica-Oblique',
        fontSize=7.2,
        leading=8.5,
        textColor=TEXT_MUTED,
        alignment=TA_CENTER,
        spaceBefore=2,
        spaceAfter=4
    )

    return {
        'title': title_style,
        'subtitle': subtitle_style,
        'h1': h1_style,
        'h2': h2_style,
        'body': body_style,
        'body_justify': body_justify,
        'bullet': bullet_style,
        'callout': callout_style,
        'equation': equation_style,
        'th': table_header_style,
        'td': table_cell_style,
        'td_mono': table_cell_mono,
        'td_center': table_cell_center,
        'caption': caption_style
    }

def make_callout(text, title="NOTE / CRITICAL SPECIFICATION", alert=False):
    """Creates a styled callout box."""
    styles = get_project_styles()
    bg_color = colors.HexColor('#FDEDEC') if alert else colors.HexColor('#F0F9FF')
    border_col = ALERT_RED if alert else HIGHLIGHT_BLUE
    title_col = ALERT_RED if alert else PRIMARY_NAVY
    
    content = [
        Paragraph(f"<b>{title}</b>", ParagraphStyle('CT', fontName='Helvetica-Bold', fontSize=7.5, leading=9.0, textColor=title_col)),
        Spacer(1, 2),
        Paragraph(text, styles['callout'])
    ]
    
    t = Table([[content]], colWidths=[515])
    t.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,-1), bg_color),
        ('BOX', (0,0), (-1,-1), 1.0, border_col),
        ('TOPPADDING', (0,0), (-1,-1), 4),
        ('BOTTOMPADDING', (0,0), (-1,-1), 4),
        ('LEFTPADDING', (0,0), (-1,-1), 8),
        ('RIGHTPADDING', (0,0), (-1,-1), 8),
    ]))
    return t

def make_equation_box(eq_text, eq_id="", description=""):
    """Creates an equation display box."""
    styles = get_project_styles()
    row = [
        Paragraph(f"<b>{eq_text}</b>", styles['equation']),
        Paragraph(f"<b>({eq_id})</b>" if eq_id else "", ParagraphStyle('EqId', fontName='Helvetica-Bold', fontSize=8, alignment=TA_RIGHT, textColor=SECONDARY_NAVY))
    ]
    t = Table([[row[0], row[1]]], colWidths=[450, 65])
    t.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,-1), LIGHT_BG),
        ('BOX', (0,0), (-1,-1), 0.75, BORDER_COLOR),
        ('VALIGN', (0,0), (-1,-1), 'MIDDLE'),
        ('TOPPADDING', (0,0), (-1,-1), 3),
        ('BOTTOMPADDING', (0,0), (-1,-1), 3),
        ('LEFTPADDING', (0,0), (-1,-1), 6),
        ('RIGHTPADDING', (0,0), (-1,-1), 6),
    ]))
    res = [t]
    if description:
        res.append(Spacer(1, 2))
        res.append(Paragraph(f"<i>Where:</i> {description}", ParagraphStyle('EqDesc', fontName='Helvetica', fontSize=7.0, leading=8.5, textColor=TEXT_MUTED)))
    return res

def make_header_divider(title_text):
    """Creates a section header with an accent underline."""
    styles = get_project_styles()
    return [
        Paragraph(title_text, styles['h1']),
        HRFlowable(width="100%", thickness=1.5, color=ACCENT_CYAN, spaceBefore=1, spaceAfter=4)
    ]
