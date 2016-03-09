# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('parking', '0003_area_area_type'),
    ]

    operations = [
        migrations.AlterField(
            model_name='area',
            name='area_type',
            field=models.CharField(default=b'12', max_length=2, verbose_name=b'area zoom', choices=[(b'12', b'City'), (b'14', b'Area'), (b'17', b'Parking')]),
        ),
    ]
