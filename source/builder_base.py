# builder_base.py - Base engine for generating exact 40-page technical volumes
import os, sys
from reportlab.lib.pagesizes import A4
from reportlab.lib import colors
from reportlab.platypus import SimpleDocTemplate, Paragraph, Spacer, Table, TableStyle, PageBreak, Image, HRFlowable
from reportlab.lib.styles import ParagraphStyle
from reportlab.lib.enums import TA_CENTER, TA_LEFT, TA_RIGHT, TA_JUSTIFY
from pypdf import PdfReader

sys.path.insert(0, os.path.dirname(__file__))
from reportlab_engine import (
    NumberedCanvas, get_project_styles, make_callout, make_equation_box, make_header_divider,
    PRIMARY_NAVY, SECONDARY_NAVY, ACCENT_CYAN, HIGHLIGHT_BLUE, LIGHT_BG, ALT_ROW_BG, BORDER_COLOR, ALERT_RED, TEXT_DARK, TEXT_MUTED
)

def build_40page_pdf(pdf_path, cover_info, pages_data):
    """
    Builds an exact 40-page PDF document.
    cover_info: dict with title, subtitle, doc_id, date, primary_exp, fig_path, etc.
    pages_data: list of exactly 39 page dictionaries (for pages 2 through 40).
    """
    if len(pages_data) != 39:
        raise ValueError(f"pages_data must contain exactly 39 entries, got {len(pages_data)}")
        
    styles = get_project_styles()
    doc = SimpleDocTemplate(
        pdf_path,
        pagesize=A4,
        leftMargin=40,
        rightMargin=40,
        topMargin=55,
        bottomMargin=55
    )
    story = []
    
    # ----------------------------------------------------
    # PAGE 1: COVER PAGE
    # ----------------------------------------------------
    story.append(Spacer(1, 20))
    banner_text = cover_info.get('banner', 'ADVANCED SPACETIME ENGINEERING RESEARCH PROJECT | NASA-BENCHMARKED TRACEABILITY')
    banner_data = [[
        Paragraph(f"<font color='white'><b>{banner_text}</b></font>", styles['th'])
    ]]
    t_banner = Table(banner_data, colWidths=[515])
    t_banner.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,-1), PRIMARY_NAVY),
        ('ALIGN', (0,0), (-1,-1), 'CENTER'),
        ('TOPPADDING', (0,0), (-1,-1), 7),
        ('BOTTOMPADDING', (0,0), (-1,-1), 7),
    ]))
    story.append(t_banner)
    story.append(Spacer(1, 25))
    
    story.append(Paragraph(cover_info['main_title'], ParagraphStyle('CoverMain', fontName='Helvetica-Bold', fontSize=22, leading=26, textColor=PRIMARY_NAVY)))
    story.append(Spacer(1, 4))
    story.append(Paragraph(cover_info['vol_title'], ParagraphStyle('CoverVol', fontName='Helvetica-Bold', fontSize=13, leading=17, textColor=SECONDARY_NAVY)))
    story.append(HRFlowable(width='100%', thickness=2.5, color=ACCENT_CYAN, spaceBefore=6, spaceAfter=10))
    story.append(Paragraph(cover_info['subtitle'], styles['subtitle']))
    story.append(Spacer(1, 15))
    
    fig_path = cover_info.get('fig_path')
    if fig_path and os.path.exists(fig_path):
        story.append(Image(fig_path, width=470, height=185))
        story.append(Spacer(1, 4))
        story.append(Paragraph(f"Figure 1.0: {cover_info.get('fig_caption', 'Experimental and Theoretical Architecture Baseline')}", styles['caption']))
        story.append(Spacer(1, 15))
    else:
        story.append(Spacer(1, 80))
        
    ctrl_data = [
        [Paragraph('<b>Document Identifier:</b>', styles['td']), Paragraph(cover_info.get('doc_id', 'PTM-DOC-001'), styles['td_mono']),
         Paragraph('<b>Date of Issue:</b>', styles['td']), Paragraph(cover_info.get('date', 'October 2026'), styles['td'])],
        [Paragraph('<b>Authoring Group:</b>', styles['td']), Paragraph('Spacetime Research Consortium', styles['td']),
         Paragraph('<b>Security Classification:</b>', styles['td']), Paragraph('UNCLASSIFIED / PUBLIC', styles['td'])],
        [Paragraph('<b>Primary Experiment:</b>', styles['td']), Paragraph(cover_info.get('primary_exp', 'Differential Relativistic Metric Platform'), styles['td']),
         Paragraph('<b>Verification Level:</b>', styles['td']), Paragraph('NASA-STD Traceability Grade', styles['td'])],
        [Paragraph('<b>Operating Baseline:</b>', styles['td']), Paragraph(cover_info.get('baseline', 'Delta h = 100.0 m (Delta f/f = 1.091e-14)'), styles['td']),
         Paragraph('<b>Physical Page Count:</b>', styles['td']), Paragraph('Exactly 40 Pages', styles['td_mono'])]
    ]
    t_ctrl = Table(ctrl_data, colWidths=[120, 150, 115, 130])
    t_ctrl.setStyle(TableStyle([
        ('BACKGROUND', (0,0), (-1,-1), LIGHT_BG),
        ('GRID', (0,0), (-1,-1), 0.5, BORDER_COLOR),
        ('TOPPADDING', (0,0), (-1,-1), 4),
        ('BOTTOMPADDING', (0,0), (-1,-1), 4),
    ]))
    story.append(t_ctrl)
    story.append(PageBreak())
    
    # ----------------------------------------------------
    # PAGES 2 TO 40: STRUCTURED PAGES
    # ----------------------------------------------------
    for p_idx, p_data in enumerate(pages_data, start=2):
        story.extend(make_header_divider(p_data['header']))
        story.append(Spacer(1, 2))
        story.append(Paragraph(p_data['intro'], styles['body_justify']))
        story.append(Spacer(1, 4))
        
        # Subsection 1
        story.append(Paragraph(p_data['sub1_title'], styles['h2']))
        story.append(Spacer(1, 2))
        story.append(Paragraph(p_data['sub1_text'], styles['body_justify']))
        story.append(Spacer(1, 4))
        
        # Flowable Element
        ftype = p_data.get('flowable_type', 'table')
        if ftype == 'table':
            t_data = p_data['flowable_data']
            col_widths = p_data.get('col_widths', [140, 195, 180])
            t_elem = Table(t_data, colWidths=col_widths)
            t_elem.setStyle(TableStyle([
                ('BACKGROUND', (0,0), (-1,0), SECONDARY_NAVY),
                ('ROWBACKGROUNDS', (0,1), (-1,-1), [colors.white, ALT_ROW_BG]),
                ('GRID', (0,0), (-1,-1), 0.5, BORDER_COLOR),
                ('TOPPADDING', (0,0), (-1,-1), 3),
                ('BOTTOMPADDING', (0,0), (-1,-1), 3),
            ]))
            story.append(t_elem)
        elif ftype == 'equation':
            eq_text = p_data['flowable_data']['text']
            eq_id = p_data['flowable_data'].get('id', f'EQ-{p_idx}.1')
            story.extend(make_equation_box(eq_text, eq_id))
        elif ftype == 'callout':
            c_text = p_data['flowable_data']['text']
            c_title = p_data['flowable_data'].get('title', 'CRITICAL ENGINEERING PRINCIPLE')
            is_alert = p_data['flowable_data'].get('alert', False)
            story.append(make_callout(c_text, c_title, alert=is_alert))
        elif ftype == 'image':
            img_path = p_data['flowable_data']['path']
            w = p_data['flowable_data'].get('width', 450)
            h = p_data['flowable_data'].get('height', 130)
            if os.path.exists(img_path):
                story.append(Image(img_path, width=w, height=h))
                story.append(Paragraph(p_data['flowable_data'].get('caption', ''), styles['caption']))
        
        story.append(Spacer(1, 5))
        # Subsection 2
        story.append(Paragraph(p_data['sub2_title'], styles['h2']))
        story.append(Spacer(1, 2))
        story.append(Paragraph(p_data['sub2_text'], styles['body_justify']))
        story.append(Spacer(1, 4))
        
        story.append(PageBreak())
        
    # Build Document
    doc.build(story, canvasmaker=NumberedCanvas)
    reader = PdfReader(pdf_path)
    page_count = len(reader.pages)
    if page_count != 40:
        raise RuntimeError(f"Expected exactly 40 pages, but generated {page_count} pages in {pdf_path}")
    print(f"SUCCESS: {pdf_path} generated with EXACTLY {page_count} pages ({os.path.getsize(pdf_path):,} bytes)")
    return page_count
