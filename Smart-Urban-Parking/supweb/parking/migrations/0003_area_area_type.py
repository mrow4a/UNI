# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('parking', '0002_auto_20150611_1516'),
    ]

    operations = [
        migrations.AddField(
            model_name='area',
            name='area_type',
            field=models.CharField(default=b'C', max_length=1, verbose_name=b'area zoom', choices=[(b'C', b'City'), (b'A', b'Area'), (b'P', b'Parking')]),
        ),
    ]
